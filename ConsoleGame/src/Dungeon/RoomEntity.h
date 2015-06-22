#pragma once

#include <EntityComponentSystem/World/World.h>

#include <Maths/IVec2.h>

#include <GameEntities/Obstacles/DoorConstants.h>

#include "ScreenConstants.h"

enum EDoorSide
{
	EDoorSide_Top,
	EDoorSide_Bottom,
	EDoorSide_Left,
	EDoorSide_Right,
};

enum ERoomDimensions
{
	ERoomDimensions_Width			= ScreenConstants::EMapCols+1,
	ERoomDimensions_Height			= ScreenConstants::EMapRows,
	ERoomDimensions_DoorHorizOffset = (ERoomDimensions_Width - EDoorSize_Width) / 2,
	ERoomDimensions_DoorVertiOffset = ((ERoomDimensions_Height - EDoorSize_Height) / 2) + 1,
};

namespace RoomEntity
{
	Entity	Create(World& inWorld, const IVec2& inPosition);
	void	EraseWallForDoor(Entity inRoom, EDoorSide inSide);
}
