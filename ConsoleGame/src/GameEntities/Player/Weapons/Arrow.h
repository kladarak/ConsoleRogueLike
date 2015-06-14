#pragma once

class World;
struct IVec2;

namespace Arrow
{
	void Create(World& inWorld, const IVec2& inPosition, const IVec2& inDirection);
}
