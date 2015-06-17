#include <EntityComponent/Components/PlayerComponent.h>

#include <Core/Messaging/MessageBroadcaster.h>

#include <EntityComponent/Components/AnimationComponent.h>
#include <EntityComponent/Components/CollisionComponent.h>
#include <EntityComponent/Components/HealthComponent.h>
#include <EntityComponent/Components/MessageReceiverComponent.h>
#include <EntityComponent/Components/MonsterComponent.h>
#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/RenderableComponent.h>

#include <EntityComponent/Systems/CollisionSystem.h>
#include <EntityComponent/Systems/DamageZoneSystem.h>
#include <EntityComponent/Systems/PositionSystem.h>

#include <Input/InputBuffer.h>

#include <Messages/Messages.h>

#include "Items/ItemBase.h"
#include "PlayerBehaviours/PlayerBehaviourBase.h"
#include "PlayerMeshes.h"

using namespace Player;

static const float kDamagedFlashDuration	= 2.0f;
static const float kDamageFlashRate			= 10.0f;

PlayerComponent::PlayerComponent(Player::EFacingDirection inDirection) 
	: mFacingDirection				(inDirection)
	, mState						(Player::EState_Idle)
	, mUsingItemSlot				(EItemSlot_None)
	, mCurrentBehaviour				(new PlayerIdleBehaviour())
	, mLastFacingDirection			(inDirection)
	, mDamagedFlashTimeRemaining	(0.0f)
	, mLastSafePosition				(0, 0)
{
	memset(mItemSlot, 0, sizeof(mItemSlot));
}

PlayerComponent::PlayerComponent(PlayerComponent&& inRHS)
	: mIntention					(inRHS.mIntention)
	, mFacingDirection				(inRHS.mFacingDirection)
	, mState						(inRHS.mState)
	, mUsingItemSlot				(inRHS.mUsingItemSlot)
	, mCurrentBehaviour				(inRHS.mCurrentBehaviour)
	, mLastFacingDirection			(inRHS.mLastFacingDirection)
	, mDamagedFlashTimeRemaining	(inRHS.mDamagedFlashTimeRemaining)
	, mLastSafePosition				(inRHS.mLastSafePosition)
	, mInventory					(std::move(inRHS.mInventory))
{
	memcpy(mItemSlot, inRHS.mItemSlot, sizeof(mItemSlot));

	inRHS.mCurrentBehaviour = nullptr;
}

PlayerComponent::~PlayerComponent()
{
	delete mCurrentBehaviour;
	mCurrentBehaviour = nullptr;
}

void PlayerComponent::HandleInput(const InputBuffer& inBuffer)
{
	Intention intention;

	if ( inBuffer.IsPressed('a') )
	{
		intention.mMovement.mX		= -1;
		intention.mFacingDirection	= EFacingDirection_Left;
	}
	else if ( inBuffer.IsPressed('d') )
	{
		intention.mMovement.mX		= 1;
		intention.mFacingDirection	= EFacingDirection_Right;
	}
	
	if ( inBuffer.IsPressed('w') )
	{
		intention.mMovement.mY		= -1;
		intention.mFacingDirection	= EFacingDirection_Up;
	}
	else if ( inBuffer.IsPressed('s') )
	{
		intention.mMovement.mY		= 1;
		intention.mFacingDirection	= EFacingDirection_Down;
	}
	
	if (inBuffer.IsPressed(' '))
	{
		intention.mState		= EState_UseItem;
		intention.mUseItemSlot	= EItemSlot_Slot0;
	}
	else if (inBuffer.IsPressed('e'))
	{
		intention.mState		= EState_UseItem;
		intention.mUseItemSlot	= EItemSlot_Slot1;
	}

	mIntention = intention;
}

void PlayerComponent::Update(Entity inPlayer, float inFrameTime)
{
	CheckAndHandleIfInDamageZone(inPlayer);
	UpdatePosition(inPlayer);
	UpdateState(inPlayer, inFrameTime);
	UpdateAnimation(inPlayer, inFrameTime);
}

void PlayerComponent::CheckAndHandleIfInDamageZone(Entity inPlayer)
{
	auto posComp		= inPlayer.GetComponent<PositionComponent>();

	auto position		= posComp->GetPosition();
	bool isInDamageZone = DamageZoneSystem::IsDamageZone(*inPlayer.GetWorld(), inPlayer, position);
	if (isInDamageZone)
	{
		TakeDamage(inPlayer);
		
		// Go back to previous position
		posComp->SetPosition( mLastSafePosition );
	}
}

void PlayerComponent::TakeDamage(Entity inPlayer)
{
	if (mDamagedFlashTimeRemaining <= 0.0f)
	{
		mDamagedFlashTimeRemaining = kDamagedFlashDuration;
		inPlayer.GetComponent<HealthComponent>()->DecHealth();
	}
}

void PlayerComponent::UpdatePosition(Entity inPlayer)
{
	auto& intendedMovement = mIntention.mMovement;

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

		bool collidesInX	= !mCurrentBehaviour->CanMoveToPosition(inPlayer, stepXAxis);
		bool collidesInY	= !mCurrentBehaviour->CanMoveToPosition(inPlayer, stepYAxis);
		bool collidesAtDest = !mCurrentBehaviour->CanMoveToPosition(inPlayer, intendedPos);

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

		if ( mCurrentBehaviour->CanMoveToPosition(inPlayer, intendedPos) )
		{
			newPos = intendedPos;
		}
	}

	positionComp->SetPosition(newPos);
	mLastSafePosition = currentPos;
}

static void SetPlayerToIdleState(Entity inPlayer)
{
	inPlayer.GetComponent<AnimationComponent>()->SetAnimations( GetIdleAnimations() );
	inPlayer.GetComponent<CollisionComponent>()->SetCollisionMesh( kIdleCollisionMesh );
	inPlayer.GetComponent<PlayerComponent>()->SetUsingItemSlot( EItemSlot_None );
}

void PlayerComponent::UpdateState(Entity inPlayer, float inFrameTime)
{
	// Facing direction
	{
		mLastFacingDirection = mFacingDirection;
		if (mIntention.mFacingDirection != EFacingDirection_Count)
		{
			mFacingDirection = mIntention.mFacingDirection;
		}
	}
	
	EState currentState		= mState;
	EState intendedState	= mIntention.mState;
	EState nextState		= EState_Idle;

	if (intendedState == EState_UseItem)
	{
		SetPlayerToIdleState(inPlayer);
		
		auto prevItem	= GetUsingItem();
		auto newItem	= GetItemInSlot(mIntention.mUseItemSlot);

		if (nullptr != prevItem && newItem != prevItem)
		{
			prevItem->OnStoppedUsing(inPlayer);
		}

		if (nullptr != newItem)
		{
			SetUsingItemSlot(mIntention.mUseItemSlot);
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
		auto item			= GetUsingItem();
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

	mState = nextState;
}

void PlayerComponent::UpdateAnimation(Entity inPlayer, float inFrameTime)
{
	auto animationComp		= inPlayer.GetComponent<AnimationComponent>();
	auto renderableComp		= inPlayer.GetComponent<RenderableComponent>();

	if (mFacingDirection != mLastFacingDirection)
	{
		animationComp->SetSelectedAnimation( mFacingDirection, false /*ResetAnimation*/ );
	}

	if (mDamagedFlashTimeRemaining > 0.0f)
	{
		mDamagedFlashTimeRemaining -= inFrameTime;
		
		int		flashState	= (int) (mDamagedFlashTimeRemaining * kDamageFlashRate);
		bool	visible		= (flashState % 2) == 0;
		renderableComp->SetVisible(visible);
	}
	else
	{
		renderableComp->SetVisible(true);
	}
}

// Message handling
void PlayerComponent::OnAttacked(Entity inPlayer, const AttackMsg& inAttackMsg)
{
	TakeDamage(inPlayer);
	
	// Move to random position away from direction of attack
	auto posComp = inPlayer.GetComponent<PositionComponent>();
	auto currPos = posComp->GetPosition();

	auto preferredPos = currPos + inAttackMsg.mAttackDirection;
	if ( mCurrentBehaviour->CanMoveToPosition(inPlayer, preferredPos) )
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
