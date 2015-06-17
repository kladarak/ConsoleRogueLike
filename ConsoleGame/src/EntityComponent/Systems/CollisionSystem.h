#pragma once

#include <vector>
#include <Maths/IVec2.h>

class World;
class Entity;
class CollisionMesh;

namespace CollisionSystem
{
	bool				CollidesWithAnyEntity(World& inWorld, const IVec2& inPosition);
	bool				CollidesWithAnyEntity(World& inWorld, const Entity& inExceptThis, const IVec2& inPosition);
	bool				CollidesWith(const Entity& inCollidableEntity, const IVec2& inPosition);
	std::vector<Entity> GetListofCollidablesAtPosition(World& inWorld, const IVec2& inPosition);

	// Untested functions.
	std::vector<Entity> GetListofCollidablesCollidedWith(World& inWorld, const Entity& inEntity);
	bool				CollidesWith(const Entity& inLHS, const Entity& inRHS);

	std::vector<Entity> GetListofCollidablesCollidedWith(World& inWorld, const CollisionMesh& inMesh, const IVec2& inPosition);
}
