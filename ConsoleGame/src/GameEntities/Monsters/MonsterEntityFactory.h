#pragma once

class World;
struct IVec2;

namespace MonsterEntityFactory
{
	void Create(World& inWorld, const IVec2& inPosition);
}
