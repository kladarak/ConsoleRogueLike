#include "PlayerIdleBehaviour.h"

#include <EntityComponent/Components/AnimationComponent.h>
#include <EntityComponent/Components/CollisionComponent.h>
#include <EntityComponent/Components/PlayerComponent.h>

#include <GameEntities/Player/PlayerMeshes.h>

void PlayerIdleBehaviour::OnStart(Entity inPlayer)
{
	using namespace Player;

	inPlayer.GetComponent<AnimationComponent>()->SetAnimations( GetIdleAnimations() );
	inPlayer.GetComponent<CollisionComponent>()->SetDefaultCollisionMesh( kIdleCollisionMesh );
	inPlayer.GetComponent<PlayerComponent>()->SetUsingItemSlot( EItemSlot_None );
}

void PlayerIdleBehaviour::OnRestart(Entity /*inPlayer*/)
{
}

void PlayerIdleBehaviour::Update(Entity /*inPlayer*/, float /*inFrameTime*/)
{
}

void PlayerIdleBehaviour::OnFinish(Entity /*inPlayer*/)
{
}

bool PlayerIdleBehaviour::IsFinished() const
{ 
	return false;
}
