#pragma once

#include <EntityComponent/Components/EOrientation.h>

class World;

namespace OpenDoor
{
	void Create(World& inWorld, const IVec2& inPosition, EOrientation inOrientation);
}
