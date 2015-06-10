#pragma once

#include <Maths/IVec2.h>

class World;
class Entity;

namespace CollisionSystem
{
	bool CollidesWithAnyEntity(World& inWorld, const Entity& inExceptThis, const IVec2& inPosition);
	bool CollidesWith(const Entity& inCollidableEntity, const IVec2& inPosition);
}
