#include <EntityComponent/Components/PlayerComponent.h>

#include <EntityComponent/Components/AnimationComponent.h>
#include <EntityComponent/Components/CollisionComponent.h>
#include <EntityComponent/Components/HealthComponent.h>
#include <EntityComponent/Components/MessageReceiverComponent.h>
#include <EntityComponent/Components/MonsterComponent.h>
#include <EntityComponent/Components/OrientationComponent.h>
#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/RenderableComponent.h>

#include <EntityComponent/Systems/CollisionSystem.h>
#include <EntityComponent/Systems/PositionSystem.h>

#include <Input/InputBuffer.h>

#include <Messages/Messages.h>

#include "Items/ItemBase.h"
#include "PlayerMeshes.h"
#include "PlayerBehaviours/PlayerBehaviourBase.h"

using namespace Player;

static const float kDamagedFlashDuration	= 2.0f;
static const float kDamageFlashRate			= 10.0f;

PlayerComponent::PlayerComponent(Player::EFacingDirection inDirection) 
	: mFacingDirection				(inDirection)
	, mState						(Player::EState_Idle)
	, mUsingItemSlot				(EItemSlot_None)
	, mIdleBehaviour				(new PlayerIdleBehaviour())
	, mCurrentBehaviour				(nullptr)
	, mLastFacingDirection			(inDirection)
	, mLastSafePosition				(0, 0)
	, mDamagedFlashTimeRemaining	(0.0f)
{
	memset(mItemSlot, 0, sizeof(mItemSlot));
}

PlayerComponent::PlayerComponent(PlayerComponent&& inRHS)
	: mIntention					(inRHS.mIntention)
	, mFacingDirection				(inRHS.mFacingDirection)
	, mState						(inRHS.mState)
	, mUsingItemSlot				(inRHS.mUsingItemSlot)
	, mIdleBehaviour				(inRHS.mIdleBehaviour)
	, mCurrentBehaviour				(inRHS.mCurrentBehaviour)
	, mLastFacingDirection			(inRHS.mLastFacingDirection)
	, mLastSafePosition				(inRHS.mLastSafePosition)
	, mDamagedFlashTimeRemaining	(inRHS.mDamagedFlashTimeRemaining)
	, mInventory					(std::move(inRHS.mInventory))
{
	memcpy(mItemSlot, inRHS.mItemSlot, sizeof(mItemSlot));

	inRHS.mIdleBehaviour	= nullptr;
	inRHS.mCurrentBehaviour = nullptr;
}

PlayerComponent::~PlayerComponent()
{
	delete mIdleBehaviour;
	mIdleBehaviour = nullptr;
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
	
	if ( inBuffer.IsPressed(' ') )
	{
		intention.mState		= EState_UseItem;
		intention.mUseItemSlot	= EItemSlot_Slot0;
	}
	else if ( inBuffer.IsPressed('e') )
	{
		intention.mState		= EState_UseItem;
		intention.mUseItemSlot	= EItemSlot_Slot1;
	}

	mIntention = intention;
}

void PlayerComponent::Update(Entity inPlayer, float inFrameTime)
{
	UpdateState(inPlayer);
	UpdateOrientation(inPlayer);
	UpdatePosition(inPlayer);
	UpdateBehaviour(inPlayer, inFrameTime);
	UpdateAnimation(inPlayer, inFrameTime);
}

void PlayerComponent::UpdateState(Entity inPlayer)
{
	if (nullptr != mCurrentBehaviour && mCurrentBehaviour->IsFinished())
	{
		mCurrentBehaviour->OnFinish(inPlayer);
		mCurrentBehaviour = nullptr;
	}
	
	if (nullptr == mCurrentBehaviour)
	{
		mState				= EState_Idle;
		mCurrentBehaviour	= mIdleBehaviour;
		mCurrentBehaviour->OnStart(inPlayer);
	}

	PlayerBehaviourBase* newBehaviour = nullptr;

	switch (mIntention.mState)
	{
		case EState_UseItem:
		{
			auto item = GetItemInSlot(mIntention.mUseItemSlot);
			if (nullptr != item)
			{
				newBehaviour	= item->GetPlayerBehaviour();
				mUsingItemSlot	= mIntention.mUseItemSlot;
				mState			= mIntention.mState;
			}
			break;
		}

		case EState_Idle:
		default:
			break;
	}

	if (nullptr != newBehaviour)
	{
		if (newBehaviour == mCurrentBehaviour)
		{
			mCurrentBehaviour->OnRestart(inPlayer);
		}
		else
		{
			mCurrentBehaviour->OnFinish(inPlayer);
			mCurrentBehaviour = newBehaviour;
			mCurrentBehaviour->OnStart(inPlayer);
		}
	}
}

void PlayerComponent::UpdateOrientation(Entity inPlayer)
{
	// TODO: Remove facing directon completely.
	mLastFacingDirection = mFacingDirection;
	if (mIntention.mFacingDirection != EFacingDirection_Count)
	{
		mFacingDirection = mIntention.mFacingDirection;
	}
	
	inPlayer.GetComponent<OrientationComponent>()->SetOrientation( (EOrientation) mFacingDirection );
}

void PlayerComponent::UpdatePosition(Entity inPlayer)
{
	auto& movement = mIntention.mMovement;

	if (movement == IVec2(0, 0))
	{
		return;
	}

	auto		positionComp	= inPlayer.GetComponent<PositionComponent>();
	const IVec2	currentPos		= positionComp->GetPosition();
	IVec2		newPos			= currentPos;

	if (movement.mX != 0 && movement.mY != 0)
	{
		// attempt to move diagonally.
		IVec2 stepXAxis		= currentPos + IVec2(movement.mX, 0);
		IVec2 stepYAxis		= currentPos + IVec2(0, movement.mY);
		IVec2 intendedPos	= currentPos + movement;

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
		IVec2 intendedPos = currentPos + movement;

		if ( mCurrentBehaviour->CanMoveToPosition(inPlayer, intendedPos) )
		{
			newPos = intendedPos;
		}
	}

	positionComp->SetPosition(newPos);
	mLastSafePosition = currentPos;
}

void PlayerComponent::UpdateBehaviour(Entity inPlayer, float inFrameTime)
{
	mCurrentBehaviour->Update(inPlayer, inFrameTime);
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
	// TODO: Return an enum, e.g., EOnAttackResult_Injured, or something?
	bool wasInjured = mCurrentBehaviour->OnAttacked(inPlayer, inAttackMsg);

	if (mDamagedFlashTimeRemaining <= 0.0f && wasInjured)
	{
		mDamagedFlashTimeRemaining = kDamagedFlashDuration;
		inPlayer.GetComponent<HealthComponent>()->DecHealth();
	}
}
