#include "PlayerUpdater.h"

#include <Core/Messaging/MessageBroadcaster.h>

#include <EntityComponentSystem/World/World.h>

#include <EntityComponent/Components/AnimationComponent.h>
#include <EntityComponent/Components/HealthComponent.h>
#include <EntityComponent/Components/PlayerComponent.h>
#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/RenderableComponent.h>

#include <Messages/Messages.h>

#include "PlayerUpdateState.h"
#include "PlayerMeshes.h"

namespace Player
{

static const float kStateHoldTime = 0.5f;
static const float kDamagedFlashDuration = 2.0f;

void UpdatePlayer(const Entity& inPlayer, float inFrameTime, MessageBroadcaster& inMsgBroadcaster)
{
	auto playerComp			= inPlayer.GetComponent<PlayerComponent>();
	auto updateState		= inPlayer.GetComponent<PlayerUpdateState>();
	
	auto state				= playerComp->GetState();
	auto facingDirection	= playerComp->GetFacingDirection();

	updateState->mTimeInState += inFrameTime;

	if (state != updateState->mLastState)
	{
		updateState->mTimeInState = 0.0f;
	}
	else if (updateState->mTimeInState > kStateHoldTime)
	{
		state = EState_Idle;
		playerComp->SetState(state);
		updateState->mTimeInState = 0.0f;
	}

	if (state == EState_Attacking)
	{
		auto position = inPlayer.GetComponent<PositionComponent>()->GetPosition();
		switch (facingDirection)
		{
			case EFacingDirection_Left:		position.mX -= 1; break;
			case EFacingDirection_Right:	position.mX += 1; break;
			case EFacingDirection_Up:		position.mY -= 1; break;
			case EFacingDirection_Down:		position.mY += 1; break;
		}

		inMsgBroadcaster.Broadcast( PlayerAttackMsg(inPlayer, position) );
	}

	if (state != updateState->mLastState || facingDirection != updateState->mLastFacingDirection)
	{
		updateState->mLastState				= state;
		updateState->mLastFacingDirection	= facingDirection;

		int animationSelection = 0;
		
		switch (state)
		{
			case EState_Idle:		animationSelection = 0; break;
			case EState_Attacking:	animationSelection = 4;	break;
			case EState_Defending:	animationSelection = 8;	break;
			default:				animationSelection = 0;	break;
		}
		
		animationSelection += (int) facingDirection;

		inPlayer.GetComponent<AnimationComponent>()->SetSelectedAnimation(animationSelection, true);
	}

	if (updateState->mDamagedFlashTimeRemaining > 0.0f)
	{
		updateState->mDamagedFlashTimeRemaining -= inFrameTime;
		
		float timeRemaining = updateState->mDamagedFlashTimeRemaining;
		
		int anim = (int) (timeRemaining * 4.0f);
		bool visible = (anim % 2) == 0;
		inPlayer.GetComponent<RenderableComponent>()->SetVisible(visible);
	}
	else
	{
		inPlayer.GetComponent<RenderableComponent>()->SetVisible(true);
	}
}

void OnTouchedMonster(const TouchedMonsterMsg& inMsg)
{
	auto updateState = inMsg.mPlayer.GetComponent<PlayerUpdateState>();
	updateState->mDamagedFlashTimeRemaining = kDamagedFlashDuration;

	// Go back to previous position
	auto posComp	= inMsg.mPlayer.GetComponent<PositionComponent>();
	auto oldPos		= posComp->GetPreviousPosition();
	//posComp->SwapPositionBuffers(); // Doing this might fix trigger box problems, but it seems a bit hacky...
	posComp->SetPosition(oldPos);

	inMsg.mPlayer.GetComponent<HealthComponent>()->DecHealth();
}

}
