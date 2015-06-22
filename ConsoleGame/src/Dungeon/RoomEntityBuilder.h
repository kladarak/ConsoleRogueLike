#pragma once

#include <EntityComponentSystem/World/World.h>

#include <Maths/IVec2.h>

#include <GameEntities/Obstacles/DoorConstants.h>

#include <Renderer/AsciiMesh.h>

#include "ScreenConstants.h"

enum EDoorMask
{
	EDoorMask_None		= 0,
	EDoorMask_Top		= 1 << 0,
	EDoorMask_Bottom	= 1 << 1,
	EDoorMask_Left		= 1 << 2,
	EDoorMask_Right		= 1 << 3,
};

enum ERoomDimensions
{
	ERoomDimensions_Width			= ScreenConstants::EMapCols+1,
	ERoomDimensions_Height			= ScreenConstants::EMapRows,
	ERoomDimensions_DoorHorizOffset = (ERoomDimensions_Width - EDoorSize_Width) / 2,
	ERoomDimensions_DoorVertiOffset = ((ERoomDimensions_Height - EDoorSize_Height) / 2) + 1,
};

class RoomEntityBuilder
{
public:
	RoomEntityBuilder(World& inWorld);

	RoomEntityBuilder&	operator=(const RoomEntityBuilder& inRHS) = delete;

	RoomEntityBuilder&	SetDoorMask(EDoorMask inMask);
	RoomEntityBuilder&	SetPosition(const IVec2& inPosition);

	Entity				Create() const;

private:
	static AsciiMesh	sGenerateRoom();
	static void			sSetupDoors(AsciiMesh& inMesh, EDoorMask inDoorMask);

	World&				mWorld;
	IVec2				mPosition;
	EDoorMask			mDoorMask;
};
