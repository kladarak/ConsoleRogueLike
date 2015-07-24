#pragma once

#include <EntityComponentSystem/World/World.h>
#include <Containers/Dynamic2DVector.h>
#include <EntityComponent/Components/EOrientation.h>

enum EDoorSide
{
	EDoorSide_Top,
	EDoorSide_Bottom,
	EDoorSide_Left,
	EDoorSide_Right,

	EDoorSide_Count
};

inline IVec2 sGetNeighbourIndex(const IVec2& inIndex, EDoorSide inDoorSide)
{
	return inIndex + gGetOrientationVector((EOrientation) inDoorSide);
}

inline EDoorSide sOppositeSide(EDoorSide inDoorSide)
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

struct RoomData
{
	bool mIsValid;
	bool mDoors[EDoorSide_Count];

	RoomData() { memset(this, 0, sizeof(RoomData)); }
};

class DungeonMap
{
public:
	DungeonMap() { }
	DungeonMap(const Dynamic2DVector<RoomData>& inRoomData, const Dynamic2DVector<Entity>& inRoomEntities)
		: mRoomData		(inRoomData)
		, mRoomEntities	(inRoomEntities)
		, mRoomsVisited	(inRoomEntities.GetColCount(), inRoomEntities.GetRowCount(), false)
	{
	}

	size_t								GetColCount() const									{ return mRoomEntities.GetColCount(); }
	size_t								GetRowCount() const									{ return mRoomEntities.GetRowCount(); }

	void								SetRoomVisited(size_t inCol, size_t inRow)			{ mRoomsVisited.Set(inCol, inRow, true); }

	const Dynamic2DVector<RoomData>&	GetRoomData() const									{ return mRoomData; }
	const Dynamic2DVector<Entity>&		GetRoomEntities() const								{ return mRoomEntities; }
	const Dynamic2DVector<bool>&		GetRoomsVisited() const								{ return mRoomsVisited; }

	void								SetPlayerStartingRoomIndex(const IVec2& inIndex)	{ mPlayerStartingRoomIndex = inIndex; }
	const IVec2&						GetPlayerStartingRoomIndex() const					{ return mPlayerStartingRoomIndex; }

private:
	Dynamic2DVector<RoomData>			mRoomData;
	Dynamic2DVector<Entity>				mRoomEntities;
	Dynamic2DVector<bool>				mRoomsVisited;

	IVec2								mPlayerStartingRoomIndex;
};
