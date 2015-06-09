#include "CollisionSystem.h"

#include <EntityComponentSystem/World/World.h>

#include <EntityComponent/Components/CollisionComponent.h>
#include <EntityComponent/Components/PositionComponent.h>

namespace CollisionSystem
{

bool CollidesWithAnyEntity(World* inWorld, const Entity& inExceptThis, const IVec2& inPosition)
{
	bool collides = false;
	
	inWorld->ForEachEntity( [&] (Entity inEntity)
	{
		if (inEntity == inExceptThis)
		{
			return;
		}

		if (CollidesWith(inEntity, inPosition))
		{
			collides = true;
		}
	} );

	return collides;
}

bool CollidesWith(const Entity& inCollidableEntity, const IVec2& inPosition)
{
	auto positionComp	= inCollidableEntity.GetComponent<PositionComponent>();
	auto collisionComp	= inCollidableEntity.GetComponent<CollisionComponent>();

	if (nullptr == positionComp || nullptr == collisionComp)
	{
		return false;
	}

	auto relativePos = inPosition - positionComp->GetPosition();

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
