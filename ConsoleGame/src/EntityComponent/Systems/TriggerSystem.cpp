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
		
		for (auto& triggerer : triggerers)
		{
			auto posComp		= triggerer.GetComponent<PositionComponent>();
			auto newPos			= posComp->GetPosition();
			auto oldPos			= posComp->GetPreviousPosition();

			bool wasInTrigger	= triggerBoxComp->IsInBounds(triggerBox, oldPos);
			bool isInTrigger	= triggerBoxComp->IsInBounds(triggerBox, newPos);

			if (isInTrigger && !wasInTrigger)
			{
				triggerBoxComp->OnEntered(triggerBox, triggerer);
			}
			else if (!isInTrigger && wasInTrigger)
			{
				triggerBoxComp->OnExited(triggerBox, triggerer);
			}
		}
	}
}

}
