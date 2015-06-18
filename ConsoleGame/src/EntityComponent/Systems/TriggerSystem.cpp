#include "TriggerSystem.h"

#include <EntityComponentSystem/World/World.h>
#include <EntityComponentSystem/World/EntityFilter.h>

#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/TriggerBoxComponent.h>

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
			auto posComp = triggerer.GetComponent<PositionComponent>();

			if (triggerBoxComp->IsInBounds(triggerBox, posComp->GetPosition()))
			{
				entitiesInBounds.push_back(triggerer);
			}
		}
		
		// Swap vectors
		auto entitiesInBoundsLastFrame = triggerBoxComp->GetEntitiesInBoundsLastFrame();
		triggerBoxComp->SetEntitiesInBoundsLastFrame(entitiesInBounds);
		
		// Now do callbacks.
		// Entity IDs should be in order, so walk over both lists, incrementing as we go.
		auto thisFrameBegin = entitiesInBounds.begin();
		auto thisFrameEnd	= entitiesInBounds.end();
		auto lastFrameBegin = entitiesInBoundsLastFrame.begin();
		auto lastFrameEnd	= entitiesInBoundsLastFrame.end();

		while (thisFrameBegin != thisFrameEnd || lastFrameBegin != lastFrameEnd)
		{
			if (thisFrameBegin != thisFrameEnd && lastFrameBegin != lastFrameEnd)
			{
				auto& entityThis = *thisFrameBegin;
				auto& entityLast = *lastFrameBegin;
				if (entityThis == entityLast)
				{
					++thisFrameBegin;
					++lastFrameBegin;
				}
				else if (entityThis.GetID() < entityLast.GetID())
				{
					triggerBoxComp->OnEntered(triggerBox, entityThis);
					++thisFrameBegin;
				}
				else // if (entityThis.GetID() > entityLast.GetID())
				{
					triggerBoxComp->OnExited(triggerBox, entityLast);
					++lastFrameBegin;
				}
			}
			else if (thisFrameBegin != thisFrameEnd)
			{
				triggerBoxComp->OnEntered(triggerBox, *thisFrameBegin);
				++thisFrameBegin;
			}
			else //if (lastFrameBegin != lastFrameEnd)
			{
				triggerBoxComp->OnExited(triggerBox, *lastFrameBegin);
				++lastFrameBegin;
			}
		}
	}
}

}
