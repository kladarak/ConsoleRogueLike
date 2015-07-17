#include "PlayerDeadBehaviour.h"

#include <Containers/ContainerMacros.h>

#include <EntityComponent/Components/AnimationComponent.h>
#include <EntityComponent/Components/CollisionComponent.h>

	
static const AsciiMesh		kDeadRenderMesh( Fragment('X', ETextDarkYellow) );
static const CollisionMesh	kDeadCollisionMesh( 0, 0 );

static const Animation kDeadAnimations[] =
{
	Animation( &kDeadRenderMesh, 1, 0.0f, Animation::EPlaybackStyle_Once ),
	Animation( &kDeadRenderMesh, 1, 0.0f, Animation::EPlaybackStyle_Once ),
	Animation( &kDeadRenderMesh, 1, 0.0f, Animation::EPlaybackStyle_Once ),
	Animation( &kDeadRenderMesh, 1, 0.0f, Animation::EPlaybackStyle_Once ),
};

void PlayerDeadBehaviour::OnStart(Entity inPlayer)
{
	using namespace Player;

	inPlayer.GetComponent<AnimationComponent>()->SetAnimations( gCArrayToVector(kDeadAnimations, gElemCount(kDeadAnimations)) );
	inPlayer.GetComponent<CollisionComponent>()->SetDefaultCollisionMesh( kDeadCollisionMesh );
}

void PlayerDeadBehaviour::OnRestart(Entity /*inPlayer*/)
{
}

void PlayerDeadBehaviour::Update(Entity /*inPlayer*/, float /*inFrameTime*/)
{
}

void PlayerDeadBehaviour::OnFinish(Entity /*inPlayer*/)
{
}

bool PlayerDeadBehaviour::IsFinished() const
{ 
	return false;
}
