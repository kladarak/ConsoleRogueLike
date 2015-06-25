#pragma once

#include <EntityComponentSystem/World/World.h>
#include <EntityComponent/Components/EOrientation.h>

namespace OpenDoor
{
	Entity Create(World& inWorld, const IVec2& inPosition, EOrientation inOrientation);
}
