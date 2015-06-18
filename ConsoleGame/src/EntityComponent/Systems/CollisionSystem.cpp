#include "CollisionSystem.h"

#include <Core/Debug/Assert.h>

#include <EntityComponentSystem/World/World.h>
#include <EntityComponentSystem/World/EntityFilter.h>

#include <EntityComponent/Components/CollisionComponent.h>
#include <EntityComponent/Components/OrientationComponent.h>
#include <EntityComponent/Components/PositionComponent.h>

namespace CollisionSystem
{

static std::vector<Entity> sGetCollidableEntities(World& inWorld)
{
	return inWorld.GetEntities( EntityFilter().MustHave<CollisionComponent>() );
}

static const IVec2& sGetPosition(const Entity& inEntity)
{
	auto positionComp = inEntity.GetComponent<PositionComponent>();
	return (nullptr != positionComp) ? positionComp->GetPosition() : IVec2::sZero();
}

//-----------------------------------------------------------------------------------------------------------------------

const CollisionMesh& GetActiveCollisionMesh(const Entity& inEntity)
{
	auto collisionComp = inEntity.GetComponent<CollisionComponent>();
	assert(nullptr != collisionComp);

	if (inEntity.HasComponent<OrientationComponent>())
	{
		auto orientation = inEntity.GetComponent<OrientationComponent>()->GetOrientation();
		return collisionComp->GetCollisionMesh( orientation );
	}
	else
	{
		return collisionComp->GetDefaultCollisionMesh();
	}
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
	if (!inCollidableEntity.HasComponent<CollisionComponent>())
	{
		return false;
	}

	auto& mesh		= GetActiveCollisionMesh(inCollidableEntity);
	auto& position	= sGetPosition(inCollidableEntity);

	auto relativePos = inPosition - position;
	return mesh.CollidesWith( relativePos );
}

bool CollidesWithAnyEntity(World& inWorld, const Entity& inEntity)
{
	return GetListofCollidablesCollidedWith(inWorld, inEntity).size() > 0;
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
	auto lhsCollisionComp	= inLHS.GetComponent<CollisionComponent>();
	auto rhsCollisionComp	= inRHS.GetComponent<CollisionComponent>();
	
	// No collision unless both entities have collision components.
	if (nullptr == lhsCollisionComp || nullptr == rhsCollisionComp)
	{
		return false;
	}
	
	auto lhsPos	= sGetPosition(inLHS);
	auto rhsPos	= sGetPosition(inRHS);
	auto& lhsMesh = GetActiveCollisionMesh(inLHS);
	auto& rhsMesh = GetActiveCollisionMesh(inRHS);

	bool collides = false;

	lhsMesh.ForEachCollidablePosition( [&] (int inX, int inY)
	{
		if (!collides)
		{
			IVec2 collPos(inX, inY);
			collPos += lhsPos;
			collPos -= rhsPos;
			collides = rhsMesh.CollidesWith(collPos);
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
