#include "DungeonLayoutGenerator.h"

#include <EntityComponent/Components/EOrientation.h>

namespace
{
	IVec2 sGetNeighbourIndex(const IVec2& inIndex, EDoorSide inDoorSide)
	{
		return inIndex + gGetOrientationVector((EOrientation) inDoorSide);
	}

	EDoorSide sOppositeSide(EDoorSide inDoorSide)
	{
		switch (inDoorSide)
		{
			case EDoorSide_Top:		return EDoorSide_Bottom;
			case EDoorSide_Bottom:	return EDoorSide_Top;
			case EDoorSide_Left:	return EDoorSide_Right;
			case EDoorSide_Right:	return EDoorSide_Left;
			default: assert(false); return inDoorSide;
		}
	}

	bool sRandomBool()
	{
		return ((rand() % 2) == 0);
	};
}

DungeonLayoutGenerator::DungeonLayoutGenerator(uint32_t inRoomCount)
	: mTargetRoomCount(inRoomCount)
	, mCurrentRoomCount(0)
{
}

Dynamic2DVector<RoomData> DungeonLayoutGenerator::Generate()
{
	/*
	- Pick random starting point.
	- Pick a random direction.
	- If there's a room already there, add a door into it (or randomly decide to), and try again.
	- If not, create a room and move into it.
	- If all directions have rooms, pop back one room.
	*/

	mLayout.Clear();
	mCurrentRoomCount = 0;

	IVec2 seedPosition( 5, 5 );
	RecurseIntoRoom(seedPosition, EDoorSide_Count);

	return mLayout;
}

void DungeonLayoutGenerator::RecurseIntoRoom(const IVec2& inPosition, EDoorSide inFromSide)
{
	{
		RoomData thisRoom;
		thisRoom.mIsValid = true;

		if (inFromSide != EDoorSide_Count)
		{
			thisRoom.mDoors[inFromSide] = true;
		}

		mLayout.Set(inPosition, thisRoom);
		++mCurrentRoomCount;

		// Expand dungeon if necessary
		auto southEastPos = inPosition + IVec2(1, 1);
		if ((int) mLayout.GetColCount() <= southEastPos.mX || (int) mLayout.GetRowCount() <= southEastPos.mY)
		{
			mLayout.Set(southEastPos, RoomData()); 
		}
	}
	
	while (mCurrentRoomCount != mTargetRoomCount)
	{
		{
			bool allDirectionsVisited = true;

			for (int i = 0; i < EDoorSide_Count; ++i)
			{
				auto neighbourPos		= sGetNeighbourIndex(inPosition, (EDoorSide) i);
				bool directionVisited	= (neighbourPos.mX < 0 || neighbourPos.mY < 0 || mLayout.Get(neighbourPos).mIsValid);
				allDirectionsVisited &= directionVisited;
			}

			if (allDirectionsVisited)
			{
				return;
			}
		}

		EDoorSide doorSide = EDoorSide_Top;
		IVec2 neighbourPos(-1, -1);
		while (neighbourPos.mX < 0 || neighbourPos.mY < 0)
		{
			doorSide = (EDoorSide) (rand() % 4);
			neighbourPos = sGetNeighbourIndex(inPosition, doorSide);
		}

		if (mLayout.Get(neighbourPos).mIsValid)
		{
			if (sRandomBool())
			{
				// Add a door into this, and try another direction.
				mLayout.Get(inPosition).mDoors[doorSide] = true;
				mLayout.Get(neighbourPos).mDoors[ sOppositeSide(doorSide) ] = true;
			}
		}
		else
		{
			mLayout.Get(inPosition).mDoors[doorSide] = true;
			RecurseIntoRoom(neighbourPos, sOppositeSide(doorSide));
		}
	}
}
