#include "AnimationSystem.h"

#include <EntityComponentSystem/World/World.h>
#include <EntityComponentSystem/World/EntityFilter.h>

#include <EntityComponent/Components/AnimationComponent.h>
#include <EntityComponent/Components/RenderableComponent.h>


namespace AnimationSystem
{

void Update(World& inWorld, float inFrameTime)
{
	auto entities = inWorld.GetEntities( EntityFilter().MustHave<AnimationComponent>().MustHave<RenderableComponent>() );
	for (auto& entity : entities)
	{
		auto animation	= entity.GetComponent<AnimationComponent>();
		auto renderable = entity.GetComponent<RenderableComponent>();

		animation->Update(inFrameTime);
		renderable->SetMesh( animation->GetCurrentKeyFrame() );
	}
}

}
