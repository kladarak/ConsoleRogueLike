#pragma once

#include <stdint.h>
#include <Containers/Dynamic2DVector.h>
#include <EntityComponent/Components/EOrientation.h>

#include "DungeonMap.h"

class DungeonLayoutGenerator
{
public:
	DungeonLayoutGenerator(uint32_t inRoomCount);

	Dynamic2DVector<RoomData>	Generate();

	void						operator=(const DungeonLayoutGenerator&) = delete;

private:
	void						RecurseIntoRoom(const IVec2& inPosition, EDoorSide inFromSide);

	const uint32_t				mTargetRoomCount;
	uint32_t					mCurrentRoomCount;
	Dynamic2DVector<RoomData>	mLayout;
};
