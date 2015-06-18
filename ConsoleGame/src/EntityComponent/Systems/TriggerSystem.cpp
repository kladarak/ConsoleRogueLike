#include "TriggerSystem.h"

#include <EntityComponentSystem/World/World.h>
#include <EntityComponentSystem/World/EntityFilter.h>

#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/TriggerBoxComponent.h>

namespace TriggerSystem
{

template<typename TInBothFunc, typename TInLeftOnlyFunc, typename TInRightOnlyFunc>
static void WalkSortedVectors(const std::vector<Entity>&	inLHS,
							  const std::vector<Entity>&	inRHS,
							  const TInBothFunc&			inBothFunc,
							  const TInLeftOnlyFunc&		inInLeftOnlyFunc,
							  const TInRightOnlyFunc&		inInRightOnlyFunc)
{
	// Entity IDs should be in order, so walk over both lists, incrementing as we go.
	auto lhsBegin	= inLHS.begin();
	auto lhsEnd		= inLHS.end();
	auto rhsBegin	= inRHS.begin();
	auto rhsEnd		= inRHS.end();

	while (lhsBegin != lhsEnd || rhsBegin != rhsEnd)
	{
		if (lhsBegin != lhsEnd && rhsBegin != rhsEnd)
		{
			auto& left = *lhsBegin;
			auto& right = *rhsBegin;
			if (left == right)
			{
				inBothFunc(left);
				++lhsBegin;
				++rhsBegin;
			}
			else if (left.GetID() < right.GetID())
			{
				inInLeftOnlyFunc(left);
				++lhsBegin;
			}
			else // if (left.GetID() > right.GetID())
			{
				inInRightOnlyFunc(right);
				++rhsBegin;
			}
		}
		else if (lhsBegin != lhsEnd)
		{
			inInLeftOnlyFunc(*lhsBegin);
			++lhsBegin;
		}
		else //if (rhsBegin != rhsEnd)
		{
			inInRightOnlyFunc(*rhsBegin);
			++rhsBegin;
		}
	}
}

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
