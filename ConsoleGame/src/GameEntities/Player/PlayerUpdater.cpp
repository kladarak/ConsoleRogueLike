#include "PlayerUpdater.h"

#include <Core/Messaging/MessageBroadcaster.h>

#include <EntityComponentSystem/World/World.h>

#include <EntityComponent/Components/AnimationComponent.h>
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

static const float kStateHoldTime			= 0.5f;
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
	auto collidablesAtPosition = CollisionSystem::GetListofCollidablesAtPosition(*inPlayer.GetWorld(), inPosition);
	for (auto& collidable : collidablesAtPosition)
	{
		if (!collidable.HasComponent<MonsterComponent>())
		{
			return false;
		}
	}

	return true;
}

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

static void UpdateState(Entity inPlayer, float inFrameTime)
{
	auto playerComp		= inPlayer.GetComponent<PlayerComponent>();
	auto updateState	= inPlayer.GetComponent<PlayerUpdateState>();

	updateState->mTimeInState += inFrameTime;

	if (playerComp->GetState() != updateState->mLastState)
	{
		updateState->mTimeInState = 0.0f;
	}
	else if (updateState->mTimeInState > kStateHoldTime)
	{
		playerComp->SetState(EState_Idle);
		updateState->mTimeInState = 0.0f;
	}
}

static void UpdatePosition(const Entity& inPlayer)
{
	auto playerComponent = inPlayer.GetComponent<PlayerComponent>();
	IVec2 intendedMovement = playerComponent->GetIntendedMovement();
	playerComponent->SetIntendedMovement( IVec2(0, 0) );

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

static void UseItem(Entity inPlayer)
{
	auto playerComp		= inPlayer.GetComponent<PlayerComponent>();
	auto state			= playerComp->GetState();
	
	switch (state)
	{
		case EState_StartUsingItem1:
		case EState_UsingItem1:
		{
			auto item = playerComp->GetItemInSlot1();
			if (nullptr != item)
			{	
				bool startedThisFrame = state == EState_StartUsingItem1;
				item->Use(inPlayer, startedThisFrame);
				playerComp->SetState(EState_UsingItem1);
			}
			else
			{
				playerComp->SetState(EState_Idle);
			}
			break;
		}

		case EState_StartUsingItem2:
		case EState_UsingItem2:
		{
			auto item = playerComp->GetItemInSlot2();
			if (nullptr != item)
			{	
				bool startedThisFrame = state == EState_StartUsingItem2;
				item->Use(inPlayer, startedThisFrame);
				playerComp->SetState(EState_UsingItem2);
			}
			else
			{
				playerComp->SetState(EState_Idle);
			}
			break;
		}

		default:
			break;
	}
}

static void UpdateAnimation(Entity inPlayer, float inFrameTime)
{
	auto playerComp			= inPlayer.GetComponent<PlayerComponent>();
	auto updateState		= inPlayer.GetComponent<PlayerUpdateState>();
	auto animationComp		= inPlayer.GetComponent<AnimationComponent>();

	auto state				= playerComp->GetState();
	auto facingDirection	= playerComp->GetFacingDirection();

	if (state != updateState->mLastState)
	{
		std::vector<Animation> animations = [&] ()
		{
			switch (playerComp->GetState())
			{
				case EState_StartUsingItem1:
				case EState_UsingItem1:
				{
					auto item = playerComp->GetItemInSlot1();
					return (nullptr != item) ? item->GetAnimations() : GetAnimations();
				}

				case EState_StartUsingItem2:
				case EState_UsingItem2:
				{
					auto item = playerComp->GetItemInSlot2();
					return (nullptr != item) ? item->GetAnimations() : GetAnimations();
				}

				default:
					return GetAnimations();
			}
		} ();

		animationComp->SetAnimations(animations);
	}
	
	updateState->mLastState = state;

	if (facingDirection != updateState->mLastFacingDirection)
	{
		updateState->mLastFacingDirection = facingDirection;
		animationComp->SetSelectedAnimation( facingDirection, true /*ResetAnimation*/ );
	}

	if (updateState->mDamagedFlashTimeRemaining > 0.0f)
	{
		updateState->mDamagedFlashTimeRemaining -= inFrameTime;
		
		float timeRemaining = updateState->mDamagedFlashTimeRemaining;
		
		int anim = (int) (timeRemaining * kDamageFlashRate);
		bool visible = (anim % 2) == 0;
		inPlayer.GetComponent<RenderableComponent>()->SetVisible(visible);
	}
	else
	{
		inPlayer.GetComponent<RenderableComponent>()->SetVisible(true);
	}
}

void UpdatePlayer(const Entity& inPlayer, float inFrameTime)
{
	CheckAndHandleIfInDamageZone(inPlayer);
	UpdateState(inPlayer, inFrameTime);
	UpdatePosition(inPlayer);
	UseItem(inPlayer);
	UpdateAnimation(inPlayer, inFrameTime);
}

}
