#include "PlayerInputHandler.h"

#include <EntityComponentSystem/World/World.h>

#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/PlayerComponent.h>

#include <EntityComponent/Systems/CollisionSystem.h>

#include <Input/InputBuffer.h>

#include "PlayerUpdateState.h"

namespace Player
{
	
struct PlayerIntention
{
	IVec2						mMovement;
	Player::EFacingDirection	mFacingDirection;
	Player::EState				mState;

	PlayerIntention() 
		: mMovement			(0, 0)
		, mFacingDirection	(Player::EFacingDirection_Count)
		, mState			(Player::EState_Idle)
	{
	}
};

static PlayerIntention GetPlayerIntention(const InputBuffer& inBuffer)
{
	PlayerIntention intention;

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
		intention.mState = EState_Attacking;
	}
	else if (inBuffer.IsPressed('e'))
	{
		intention.mState = EState_Defending;
	}

	return intention;
}

static void UpdatePosition(const Entity& inThis, const IVec2& inMovement)
{
	auto		positionComp	= inThis.GetComponent<PositionComponent>();
	const IVec2	currentPos		= positionComp->GetPosition();
	IVec2		newPos			= currentPos;

	if (inMovement.mX != 0 && inMovement.mY != 0)
	{
		// attempt to move diagonally.
		IVec2 stepXAxis		= currentPos + IVec2(inMovement.mX, 0);
		IVec2 stepYAxis		= currentPos + IVec2(0, inMovement.mY);
		IVec2 intendedPos	= currentPos + inMovement;

		bool collidesInX	= CollisionSystem::CollidesWithAnyEntity(*inThis.GetWorld(), inThis, stepXAxis);
		bool collidesInY	= CollisionSystem::CollidesWithAnyEntity(*inThis.GetWorld(), inThis, stepYAxis);
		bool collidesAtDest = CollisionSystem::CollidesWithAnyEntity(*inThis.GetWorld(), inThis, intendedPos);

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
		IVec2	intendedPos	= currentPos + inMovement;
		bool	collides	= CollisionSystem::CollidesWithAnyEntity(*inThis.GetWorld(), inThis, intendedPos);
		if (!collides)
		{
			newPos = intendedPos;
		}
	}

	positionComp->SetPosition(newPos);
}

void HandleInput(const Entity& inPlayer, const InputBuffer& inBuffer)
{
	auto	intention		= GetPlayerIntention(inBuffer);
	auto	playerComp		= inPlayer.GetComponent<PlayerComponent>();
	
	if (intention.mFacingDirection != EFacingDirection_Count)
	{
		playerComp->SetFacingDirection( intention.mFacingDirection );
	}

	if (intention.mState != EState_Idle)
	{
		playerComp->SetState( intention.mState );
	}

	if (intention.mMovement != IVec2(0, 0))
	{
		UpdatePosition(inPlayer, intention.mMovement);
	}
}

}
