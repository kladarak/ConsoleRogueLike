#pragma once

#include <EntityComponentSystem/World/World.h>

struct IVec2;

namespace PositionSystem
{
	void				SwapPositionBuffers(World& inWorld);
	std::vector<Entity> GetListOfEntitiesAtPosition(World& inWorld, Entity inExceptThis, const IVec2& inPosition);
}
