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

#include "Weapons/Weapon.h"
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

static void Attack(Entity inPlayer)
{
	auto playerComp = inPlayer.GetComponent<PlayerComponent>();
	auto weapon		= playerComp->GetSelectedWeapon();

	if (nullptr != weapon)
	{
		weapon->Attack(inPlayer, playerComp->GetStartedAttackThisFrame());
	}

	playerComp->SetStartedAttackThisFrame(false);
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
		std::vector<Animation> animations = GetAnimations();

		if (playerComp->GetState() == EState_Attacking)
		{
			auto weapon = playerComp->GetSelectedWeapon();
			if (nullptr != weapon)
			{
				animations = weapon->GetAnimations();
			}
		}

		animationComp->SetAnimations(animations);
	}

	if (state != updateState->mLastState || facingDirection != updateState->mLastFacingDirection)
	{
		updateState->mLastState				= state;
		updateState->mLastFacingDirection	= facingDirection;

		int animationSelection = 0;
		
		switch (state)
		{
			case EState_Idle:		animationSelection = 0; break;
			case EState_Attacking:	animationSelection = 0;	break; // Has a different anim set.
			case EState_Defending:	animationSelection = 4;	break;
			default:				animationSelection = 0;	break;
		}
		
		animationSelection += (int) facingDirection;

		animationComp->SetSelectedAnimation(animationSelection);
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

	if (inPlayer.GetComponent<PlayerComponent>()->GetState() == EState_Attacking)
	{
		Attack(inPlayer);
	}

	UpdateAnimation(inPlayer, inFrameTime);
}

}
