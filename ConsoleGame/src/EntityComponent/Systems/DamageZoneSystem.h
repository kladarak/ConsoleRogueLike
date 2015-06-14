#pragma once

#include <EntityComponentSystem/World/World.h>

struct IVec2;
class World;

namespace DamageZoneSystem
{
	bool IsDamageZone(World& inWorld, Entity inExceptThis, const IVec2& inPosition);
	bool IsDamageZone(World& inWorld, const IVec2& inPosition);
	bool IsDamageZone(Entity inDamageZoneEntity, const IVec2& inPosition);
}
