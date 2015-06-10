#include "CollisionSystem.h"

#include <EntityComponentSystem/World/World.h>
#include <EntityComponentSystem/World/EntityFilter.h>

#include <EntityComponent/Components/CollisionComponent.h>
#include <EntityComponent/Components/PositionComponent.h>

namespace CollisionSystem
{

bool CollidesWithAnyEntity(World& inWorld, const Entity& inExceptThis, const IVec2& inPosition)
{
	auto collidableEntities = inWorld.GetEntities( EntityFilter().MustHave<PositionComponent>().MustHave<CollisionComponent>() );

	for (auto& entity : collidableEntities)
	{
		if (entity != inExceptThis)
		{
			if (CollidesWith(entity, inPosition))
			{
				return true;
			}
		}
	}

	return false;
}

bool CollidesWith(const Entity& inCollidableEntity, const IVec2& inPosition)
{
	auto positionComp	= inCollidableEntity.GetComponent<PositionComponent>();
	auto collisionComp	= inCollidableEntity.GetComponent<CollisionComponent>();
	auto relativePos	= inPosition - positionComp->GetPosition();

	auto& collidablePositions = collisionComp->GetCollidablePositions();
	for (auto& pos : collidablePositions)
	{
		if (pos == relativePos)
		{
			return true;
		}
	}

	return false;
}

}
