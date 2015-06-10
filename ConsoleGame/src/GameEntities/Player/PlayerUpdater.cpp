#include "PlayerUpdater.h"

#include <EntityComponentSystem/World/World.h>

#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/RenderableComponent.h>
#include <EntityComponent/Components/PlayerComponent.h>

#include "PlayerUpdateState.h"
#include "PlayerMeshes.h"

namespace Player
{

static const float kStateHoldTime = 0.5f;

void UpdatePlayer(const Entity& inPlayer, float inFrameTime)
{
	auto playerComp			= inPlayer.GetComponent<PlayerComponent>();
	auto renderableComp		= inPlayer.GetComponent<RenderableComponent>();
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

	if (state != updateState->mLastState || facingDirection != updateState->mLastFacingDirection)
	{
		updateState->mLastState				= state;
		updateState->mLastFacingDirection	= facingDirection;

		auto meshes = [state] () -> const AsciiMesh*
		{
			switch (state)
			{
				case EState_Idle:		return kIdleMeshes;
				case EState_Attacking:	return kSwordMeshes;
				case EState_Defending:	return kShieldMeshes;
				default:				return kIdleMeshes;
			}
		} ();

		renderableComp->SetMesh( meshes[facingDirection] );
	}
}

}
