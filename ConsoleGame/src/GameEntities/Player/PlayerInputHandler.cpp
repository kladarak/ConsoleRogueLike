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
		playerComp->SetIntendedMovement( intention.mMovement );
	}
}

}
