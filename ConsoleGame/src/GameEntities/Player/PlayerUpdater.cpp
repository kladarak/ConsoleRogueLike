#include "PlayerUpdater.h"

#include <Core/Messaging/MessageBroadcaster.h>

#include <EntityComponentSystem/World/World.h>

#include <EntityComponent/Components/AnimationComponent.h>
#include <EntityComponent/Components/CollisionComponent.h>
#include <EntityComponent/Components/HealthComponent.h>
#include <EntityComponent/Components/MessageReceiverComponent.h>
#include <EntityComponent/Components/MonsterComponent.h>
#include <EntityComponent/Components/PlayerComponent.h>
#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/RenderableComponent.h>

#include <EntityComponent/Systems/CollisionSystem.h>
#include <EntityComponent/Systems/DamageZoneSystem.h>
#include <EntityComponent/Systems/PositionSystem.h>

#include <Messages/Messages.h>

#include "Items/ItemBase.h"
#include "PlayerUpdateState.h"
#include "PlayerMeshes.h"

namespace Player
{

static const float kDamagedFlashDuration	= 2.0f;
static const float kDamageFlashRate			= 10.0f;

static void TakeDamage(Entity inPlayer)
{
	auto updateState = inPlayer.GetComponent<PlayerUpdateState>();
	if (updateState->mDamagedFlashTimeRemaining <= 0.0f)
	{
		updateState->mDamagedFlashTimeRemaining = kDamagedFlashDuration;
		inPlayer.GetComponent<HealthComponent>()->DecHealth();
	}
}

static bool CanMoveToPosition(Entity inPlayer, const IVec2& inPosition)
{
	// TODO: Update this to consider the player having a larger collision mesh.
	auto collidablesAtPosition = CollisionSystem::GetListofCollidablesAtPosition(*inPlayer.GetWorld(), inPosition);
	for (auto& collidable : collidablesAtPosition)
	{
		if (collidable != inPlayer && !collidable.HasComponent<MonsterComponent>())
		{
			return false;
		}
	}

	return true;
}

static void CheckAndHandleIfInDamageZone(Entity inPlayer)
{
	auto posComp		= inPlayer.GetComponent<PositionComponent>();
	auto updateState	= inPlayer.GetComponent<PlayerUpdateState>();
	
	auto position		= posComp->GetPosition();
	bool isInDamageZone = DamageZoneSystem::IsDamageZone(*inPlayer.GetWorld(), inPlayer, position);
	if (isInDamageZone)
	{
		TakeDamage(inPlayer);
		
		// Go back to previous position
		posComp->SetPosition( updateState->mLastSafePosition );
	}
}

static void UpdatePosition(const Entity& inPlayer)
{
	auto	playerComponent		= inPlayer.GetComponent<PlayerComponent>();
	auto&	intention			= playerComponent->GetIntention();
	auto&	intendedMovement	= intention.mMovement;

	if (intendedMovement == IVec2(0, 0))
	{
		return;
	}

	auto		positionComp	= inPlayer.GetComponent<PositionComponent>();
	const IVec2	currentPos		= positionComp->GetPosition();
	IVec2		newPos			= currentPos;

	if (intendedMovement.mX != 0 && intendedMovement.mY != 0)
	{
		// attempt to move diagonally.
		IVec2 stepXAxis		= currentPos + IVec2(intendedMovement.mX, 0);
		IVec2 stepYAxis		= currentPos + IVec2(0, intendedMovement.mY);
		IVec2 intendedPos	= currentPos + intendedMovement;

		bool collidesInX	= !CanMoveToPosition(inPlayer, stepXAxis);
		bool collidesInY	= !CanMoveToPosition(inPlayer, stepYAxis);
		bool collidesAtDest = !CanMoveToPosition(inPlayer, intendedPos);

		if ( collidesInX && collidesInY )
		{
			// Do nothing
		}
		else if (collidesInX && collidesAtDest)
		{
			newPos = stepYAxis;
		}
		else if (collidesInY && collidesAtDest)
		{
			newPos = stepXAxis;
		}
		else
		{
			newPos = intendedPos;
		}
	}
	else
	{
		IVec2 intendedPos = currentPos + intendedMovement;

		if ( CanMoveToPosition(inPlayer, intendedPos) )
		{
			newPos = intendedPos;
		}
	}

	positionComp->SetPosition(newPos);
	inPlayer.GetComponent<PlayerUpdateState>()->mLastSafePosition = currentPos;
}

static void SetPlayerToIdleState(Entity inPlayer)
{
	inPlayer.GetComponent<AnimationComponent>()->SetAnimations( GetIdleAnimations() );
	inPlayer.GetComponent<CollisionComponent>()->SetCollisionMesh( kIdleCollisionMesh );
	inPlayer.GetComponent<PlayerComponent>()->SetUsingItemSlot( EItemSlot_None );
}

static void UpdateState(Entity inPlayer, float inFrameTime)
{
	auto playerComp		= inPlayer.GetComponent<PlayerComponent>();
	auto updateState	= inPlayer.GetComponent<PlayerUpdateState>();
	auto intention		= playerComp->GetIntention();

	EState currentState		= playerComp->GetState();
	EState intendedState	= intention.mState;
	EState nextState		= EState_Idle;
	
	updateState->mLastFacingDirection = playerComp->GetFacingDirection();

	if (intention.mFacingDirection != EFacingDirection_Count)
	{
		playerComp->SetFacingDirection( intention.mFacingDirection );
	}

	if (intendedState == EState_UseItem)
	{
		SetPlayerToIdleState(inPlayer);
		
		auto prevItem	= playerComp->GetUsingItem();
		auto newItem	= playerComp->GetItemInSlot(intention.mUseItemSlot);

		if (nullptr != prevItem && newItem != prevItem)
		{
			prevItem->OnStoppedUsing(inPlayer);
		}

		if (nullptr != newItem)
		{
			playerComp->SetUsingItemSlot(intention.mUseItemSlot);
			newItem->OnStartUsing(inPlayer);
			nextState = intendedState;
		}
		else if (currentState != EState_Idle)
		{
			nextState = EState_Idle;
		}
	}
	else if (currentState == EState_UseItem)
	{
		auto item			= playerComp->GetUsingItem();
		bool resetToIdle	= (nullptr == item);

		if (!resetToIdle)
		{
			bool isFinished = item->UpdateUsing(inPlayer, inFrameTime);
			resetToIdle		= isFinished;
			nextState		= currentState;
		}

		if (resetToIdle)
		{
			SetPlayerToIdleState(inPlayer);
			nextState = EState_Idle;
		}
	}

	playerComp->SetState(nextState);
}

static void UpdateAnimation(Entity inPlayer, float inFrameTime)
{
	auto playerComp			= inPlayer.GetComponent<PlayerComponent>();
	auto updateState		= inPlayer.GetComponent<PlayerUpdateState>();
	auto animationComp		= inPlayer.GetComponent<AnimationComponent>();
	auto renderableComp		= inPlayer.GetComponent<RenderableComponent>();

	auto facingDirection	= playerComp->GetFacingDirection();

	if (facingDirection != updateState->mLastFacingDirection)
	{
		animationComp->SetSelectedAnimation( facingDirection, false /*ResetAnimation*/ );
	}

	if (updateState->mDamagedFlashTimeRemaining > 0.0f)
	{
		updateState->mDamagedFlashTimeRemaining -= inFrameTime;
		
		float timeRemaining = updateState->mDamagedFlashTimeRemaining;
		
		int anim = (int) (timeRemaining * kDamageFlashRate);
		bool visible = (anim % 2) == 0;
		renderableComp->SetVisible(visible);
	}
	else
	{
		renderableComp->SetVisible(true);
	}
}

// Update job
void UpdatePlayer(const Entity& inPlayer, float inFrameTime)
{
	CheckAndHandleIfInDamageZone(inPlayer);
	UpdatePosition(inPlayer);
	UpdateState(inPlayer, inFrameTime);
	UpdateAnimation(inPlayer, inFrameTime);
}

// Message handling
void OnAttacked(const Entity& inPlayer, const AttackMsg& inAttackMsg)
{
	TakeDamage(inPlayer);
	
	// Move to random position away from direction of attack
	auto posComp = inPlayer.GetComponent<PositionComponent>();
	auto currPos = posComp->GetPosition();

	auto preferredPos = currPos + inAttackMsg.mAttackDirection;
	if ( CanMoveToPosition(inPlayer, preferredPos) )
	{
		posComp->SetPosition(preferredPos);
	}
	else 
	{
		static const IVec2 kRecoveryPositions[] = { IVec2(0, 1), IVec2(0, -1), IVec2(1, 0), IVec2(-1, 0) };

		for (size_t i = 0; i < gElemCount(kRecoveryPositions); ++i)
		{
			IVec2 testPos = currPos + kRecoveryPositions[i];
			if ( !CollisionSystem::CollidesWithAnyEntity(*inPlayer.GetWorld(), inPlayer, testPos) )
			{
				posComp->SetPosition(testPos);
			}
		}
	}
}

}
