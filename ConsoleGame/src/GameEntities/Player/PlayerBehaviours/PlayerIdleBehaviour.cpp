#include "PlayerIdleBehaviour.h"

#include <Containers/ContainerMacros.h>

#include <EntityComponent/Components/AnimationComponent.h>
#include <EntityComponent/Components/CollisionComponent.h>
#include <EntityComponent/Components/PlayerComponent.h>

	
static const AsciiMesh		kIdleRenderMesh( 'O' );
static const CollisionMesh	kIdleCollisionMesh( 0, 0 );

static const Animation kIdleAnimations[] =
{
	Animation( &kIdleRenderMesh, 1, 0.0f, Animation::EPlaybackStyle_Once ),
	Animation( &kIdleRenderMesh, 1, 0.0f, Animation::EPlaybackStyle_Once ),
	Animation( &kIdleRenderMesh, 1, 0.0f, Animation::EPlaybackStyle_Once ),
	Animation( &kIdleRenderMesh, 1, 0.0f, Animation::EPlaybackStyle_Once ),
};

static inline std::vector<Animation> GetIdleAnimations()
{
	return gCArrayToVector(kIdleAnimations, gElemCount(kIdleAnimations));
}

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
