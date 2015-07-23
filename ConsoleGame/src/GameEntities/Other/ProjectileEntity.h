#pragma once

#include <EntityComponentSystem/World/World.h>

class AsciiMesh;
struct IVec2;

namespace ProjectileEntity
{
	Entity Create(World& inWorld, const AsciiMesh& inMesh, const IVec2& inPosition, const IVec2& inDirection, int inAttackStrength);
}
