#pragma once

#include <EntityComponentSystem/World/World.h>

#include <EntityComponent/Components/EOrientation.h>

#include <Renderer/Colour.h>

namespace LockedDoor
{
	Entity	Create(World& inWorld, const IVec2& inPosition, EOrientation inOrientation, EColour inDoorColour);
	void	BindDoors(const Entity& inEntity0, const Entity& inEntity1);
}
