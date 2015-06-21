#include "TriggerSystem.h"

#include <EntityComponentSystem/World/World.h>
#include <EntityComponentSystem/World/EntityFilter.h>

#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/TriggerBoxComponent.h>

#include <Containers/ContainerMacros.h>

namespace TriggerSystem
{

void Update(World& inWorld)
{
	std::vector<Entity> triggerers = inWorld.GetEntities( EntityFilter().MustHave<TriggererComponent>().MustHave<PositionComponent>() );
	if (triggerers.size() == 0)
	{
		return;
	}
	
	std::vector<Entity> triggerBoxes = inWorld.GetEntities( EntityFilter().MustHave<TriggerBoxComponent>() );

	for (auto& triggerBox : triggerBoxes)
	{
		auto triggerBoxComp = triggerBox.GetComponent<TriggerBoxComponent>();
	
		std::vector<Entity> entitiesInBounds;
		
		for (auto& triggerer : triggerers)
		{
			auto pos = triggerer.GetComponent<PositionComponent>()->GetPosition();

			if (triggerer != triggerBox && triggerBoxComp->IsInBounds(triggerBox, pos))
			{
				entitiesInBounds.push_back(triggerer);
			}
		}
		
		// Swap vectors
		auto entitiesInBoundsLastFrame = triggerBoxComp->GetEntitiesInBoundsLastFrame();
		triggerBoxComp->SetEntitiesInBoundsLastFrame(entitiesInBounds);
		
		// Now do callbacks.
		// Entity IDs should be in order, so walk over both lists, incrementing as we go.
		WalkSortedVectors( entitiesInBounds, entitiesInBoundsLastFrame,
						  [] (const Entity& /*inEntity*/)	{  },
						  [&] (const Entity& inEntity)	{ triggerBoxComp->OnEntered(triggerBox, inEntity);	},
						  [&] (const Entity& inEntity)	{ triggerBoxComp->OnExited(triggerBox, inEntity);	} );
	}
}

void HandleDestroyedEntities(World& inWorld)
{
	std::vector<Entity> destroyedEntities	= inWorld.GetEntitiesQueuedForDestruction();
	std::vector<Entity> triggerBoxes		= inWorld.GetEntities( EntityFilter().MustHave<TriggerBoxComponent>() );

	for (auto& triggerBox : triggerBoxes)
	{
		auto triggerBoxComp				= triggerBox.GetComponent<TriggerBoxComponent>();
		auto& entitiesInBoundsLastFrame	= triggerBoxComp->GetEntitiesInBoundsLastFrame();

		std::vector<Entity> remainingEntitiesInBounds;

		WalkSortedVectors( destroyedEntities, entitiesInBoundsLastFrame,
						  [&] (const Entity& inEntity) { triggerBoxComp->OnExited(triggerBox, inEntity); },
						  [&] (const Entity& /*inEntity*/) { },
						  [&] (const Entity& inEntity) { remainingEntitiesInBounds.push_back(inEntity); } );

		triggerBoxComp->SetEntitiesInBoundsLastFrame(remainingEntitiesInBounds);
	}
}

}
