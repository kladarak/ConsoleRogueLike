#include "MonsterEntityFactory.h"

#include <EntityComponentSystem/World/World.h>

#include <EntityComponent/Components/CollisionComponent.h>
#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/RenderableComponent.h>
#include <EntityComponent/Components/TriggerBoxComponent.h>
#include <EntityComponent/Components/PlayerComponent.h>
#include <EntityComponent/Components/ProgramComponent.h>

#include <EntityComponent/Systems/CollisionSystem.h>

namespace MonsterEntityFactory
{

static const char	kMonsterAnimation[]	= "e@";
static const float	kAnimationDuration	= 0.5f;

class MonsterState
{
public:
	MonsterState() : mAnimationTime(0.0f), mAnimationFrame(0) { }
	~MonsterState() { }

	float	mAnimationTime;
	int		mAnimationFrame;
};

static void Update(const Entity& inThis, float inFrameTime)
{
	auto state = inThis.GetComponent<MonsterState>();
	state->mAnimationTime += inFrameTime;

	if (state->mAnimationTime > kAnimationDuration)
	{
		state->mAnimationTime -= kAnimationDuration;
		state->mAnimationFrame = (state->mAnimationFrame + 1) % (sizeof(kMonsterAnimation) - 1);

		AsciiMesh mesh;
		mesh.SetCharAtPosition(0, 0, kMonsterAnimation[state->mAnimationFrame]);
		inThis.GetComponent<RenderableComponent>()->SetMesh(mesh);
	}
}

void Create(World& inWorld, const IVec2& inPosition)
{
	auto entity = inWorld.CreateEntity();

	entity.AddComponent<PositionComponent>(inPosition);
	entity.AddComponent<MonsterState>();
	
	auto programComp = entity.AddComponent<ProgramComponent>();
	programComp->RegisterProgram( &Update );

	AsciiMesh mesh;
	mesh.SetCharAtPosition(0, 0, kMonsterAnimation[0]);
	entity.AddComponent<RenderableComponent>(mesh);
}

}
