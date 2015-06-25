#pragma once

#include <stdint.h>
#include <Containers/Dynamic2DVector.h>
#include <EntityComponent/Components/EOrientation.h>
#include "RoomEntity.h"


struct RoomData
{
	bool mIsValid;
	bool mDoors[EDoorSide_Count];

	RoomData() { memset(this, 0, sizeof(RoomData)); }
};

typedef Dynamic2DVector<RoomData> DungeonLayout;

class DungeonLayoutGenerator
{
public:
	DungeonLayoutGenerator(uint32_t inRoomCount);

	DungeonLayout	Generate();

	void operator=(const DungeonLayoutGenerator&) = delete;

private:
	void			RecurseIntoRoom(const IVec2& inPosition, EDoorSide inFromSide);

	const uint32_t	mTargetRoomCount;
	uint32_t		mCurrentRoomCount;
	DungeonLayout	mLayout;
};
