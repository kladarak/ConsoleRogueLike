#include "CollisionSystem.h"

#include <EntityComponentSystem/World/World.h>
#include <EntityComponentSystem/World/EntityFilter.h>

#include <EntityComponent/Components/CollisionComponent.h>
#include <EntityComponent/Components/PositionComponent.h>

namespace CollisionSystem
{

static std::vector<Entity> sGetCollidableEntities(World& inWorld)
{
	return inWorld.GetEntities( EntityFilter().MustHave<PositionComponent>().MustHave<CollisionComponent>() );
}

bool CollidesWithAnyEntity(World& inWorld, const IVec2& inPosition)
{
	return GetListofCollidablesAtPosition(inWorld, inPosition).size() > 0;
}

bool CollidesWithAnyEntity(World& inWorld, const Entity& inExceptThis, const IVec2& inPosition)
{
	auto collidableEntities = GetListofCollidablesAtPosition(inWorld, inPosition);

	for (auto& entity : collidableEntities)
	{
		if (entity != inExceptThis)
		{
			return true;
		}
	}

	return false;
}

std::vector<Entity> GetListofCollidablesAtPosition(World& inWorld, const IVec2& inPosition)
{
	std::vector<Entity> out;

	auto collidableEntities = sGetCollidableEntities(inWorld);

	for (auto& entity : collidableEntities)
	{
		if (CollidesWith(entity, inPosition))
		{
			out.push_back(entity);
		}
	}

	return out;
}

bool CollidesWith(const Entity& inCollidableEntity, const IVec2& inPosition)
{
	auto positionComp	= inCollidableEntity.GetComponent<PositionComponent>();
	auto collisionComp	= inCollidableEntity.GetComponent<CollisionComponent>();
	auto relativePos	= inPosition - positionComp->GetPosition();

	return collisionComp->CollidesWith( relativePos );
}

std::vector<Entity> GetListofCollidablesCollidedWith(World& inWorld, const Entity& inEntity)
{
	std::vector<Entity> out;

	inWorld.ForEachEntity( [&] (const Entity& inOther)
	{
		if (inEntity != inOther && CollidesWith(inEntity, inOther))
		{
			out.push_back(inOther);
		}
	} );

	return out;
}

bool CollidesWith(const Entity& inLHS, const Entity& inRHS)
{
	auto lhsPositionComp	= inLHS.GetComponent<PositionComponent>();
	auto rhsPositionComp	= inRHS.GetComponent<PositionComponent>();
	auto lhsCollisionComp	= inLHS.GetComponent<CollisionComponent>();
	auto rhsCollisionComp	= inRHS.GetComponent<CollisionComponent>();
	
	// Must have position components
	if (nullptr == lhsPositionComp || nullptr == rhsPositionComp)
	{
		return false;
	}
	
	// Early out if no collision components
	if (nullptr == lhsCollisionComp && nullptr == rhsCollisionComp)
	{
		return false;
	}
	
	auto lhsPos	= inLHS.GetComponent<PositionComponent>()->GetPosition();
	auto rhsPos	= inRHS.GetComponent<PositionComponent>()->GetPosition();

	if (nullptr == lhsCollisionComp)
	{
		return CollidesWith(inRHS, lhsPos);
	}
	
	if (nullptr == rhsCollisionComp)
	{
		return CollidesWith(inLHS, rhsPos);
	}

	bool collides = false;

	lhsCollisionComp->ForEachCollidablePosition( [&] (int inX, int inY)
	{
		if (!collides)
		{
			IVec2 collPos(inX, inY);
			collPos += lhsPos;
			collPos -= rhsPos;
			collides = rhsCollisionComp->CollidesWith(collPos);
		}
	} );

	return collides;
}

std::vector<Entity> GetListofCollidablesCollidedWith(World& inWorld, const CollisionMesh& inMesh, const IVec2& inPosition)
{
	std::vector<Entity> out;
	
	auto collidableEntities = sGetCollidableEntities(inWorld);
	
	for (auto& entity : collidableEntities)
	{
		bool collides = false;

		inMesh.ForEachCollidablePosition( [&] (int inMeshX, int inMeshY)
		{
			if (!collides)
			{
				auto meshPos = IVec2(inMeshX, inMeshY) + inPosition;
				collides = CollidesWith(entity, meshPos);
			}
		} );

		if (collides)
		{
			out.push_back(entity);
		}
	}

	return out;
}

}
