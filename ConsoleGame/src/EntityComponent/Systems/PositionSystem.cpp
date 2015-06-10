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

}
