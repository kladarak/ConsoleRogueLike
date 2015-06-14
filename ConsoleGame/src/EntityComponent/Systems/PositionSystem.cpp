#include "PositionSystem.h"

#include <EntityComponentSystem/World/World.h>
#include <EntityComponentSystem/World/EntityFilter.h>

#include <EntityComponent/Components/PositionComponent.h>

namespace PositionSystem
{
	
void SwapPositionBuffers(World& inWorld)
{
	auto entities = inWorld.GetEntities( EntityFilter().MustHave<PositionComponent>() );
	for (auto& entity : entities)
	{
		entity.GetComponent<PositionComponent>()->SwapPositionBuffers();
	}
}


std::vector<Entity> GetListOfEntitiesAtPosition(World& inWorld, Entity inExceptThis, const IVec2& inPosition)
{
	std::vector<Entity> out;

	auto entities = inWorld.GetEntities( EntityFilter().MustHave<PositionComponent>() );

	for (auto& entity : entities)
	{
		if ( entity != inExceptThis && entity.GetComponent<PositionComponent>()->GetPosition() == inPosition )
		{
			out.push_back(entity);
		}
	}

	return out;
}

}
