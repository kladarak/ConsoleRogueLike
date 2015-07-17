#include "SpinnerEntity.h"

#include <EntityComponentSystem/World/World.h>

#include <EntityComponent/Components/AnimationComponent.h>
#include <EntityComponent/Components/CollisionComponent.h>
#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/RenderableComponent.h>

namespace SpinnerEntity
{

static const float kStateChangeRate = 0.05f; // seconds

static const AsciiMesh kSpinnerFrames[] =
{
	AsciiMesh( Fragment('|', ETextBlue) ),
	AsciiMesh( Fragment('/', ETextBlue) ),
	AsciiMesh( Fragment('-', ETextBlue) ),
	AsciiMesh( Fragment('\\', ETextBlue) ),
};

static const Animation kSpinnerAnimation(kSpinnerFrames, sizeof(kSpinnerFrames)/sizeof(AsciiMesh), kStateChangeRate, Animation::EPlaybackStyle_Loop);

void Create(World& inWorld, const IVec2& inPos)
{
	Entity entity = inWorld.CreateEntity();

	entity.AddComponent<PositionComponent>(inPos);
	entity.AddComponent<RenderableComponent>( kSpinnerFrames[0] );
	entity.AddComponent<CollisionComponent>( CollisionMesh(0, 0) );
	entity.AddComponent<AnimationComponent>(kSpinnerAnimation);
}

}
