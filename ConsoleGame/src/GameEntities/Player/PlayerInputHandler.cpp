#include "PlayerInputHandler.h"

#include <EntityComponentSystem/World/World.h>

#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/PlayerComponent.h>

#include <EntityComponent/Systems/CollisionSystem.h>

#include <Input/InputBuffer.h>

#include "PlayerUpdateState.h"

namespace Player
{
	
static Intention GetPlayerIntention(const InputBuffer& inBuffer)
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

	return intention;
}

void HandleInput(const Entity& inPlayer, const InputBuffer& inBuffer)
{
	auto intention	= GetPlayerIntention(inBuffer);
	auto playerComp	= inPlayer.GetComponent<PlayerComponent>();
	playerComp->SetIntention(intention);
}

}
