#pragma once

#include <EntityComponentSystem/World/World.h>
#include <Containers/Dynamic2DVector.h>

enum EDoorSide
{
	EDoorSide_Top,
	EDoorSide_Bottom,
	EDoorSide_Left,
	EDoorSide_Right,

	EDoorSide_Count
};

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
		: mRoomData(inRoomData), mRoomEntities(inRoomEntities)
	{
		assert(mRoomData.GetColCount() == mRoomEntities.GetColCount());
		assert(mRoomData.GetRowCount() == mRoomEntities.GetRowCount());
	}

	size_t								GetColCount() const		{ return mRoomData.GetColCount(); }
	size_t								GetRowCount() const		{ return mRoomData.GetRowCount(); }

	const Dynamic2DVector<RoomData>&	GetRoomData() const		{ return mRoomData; }
	const Dynamic2DVector<Entity>&		GetRoomEntities() const { return mRoomEntities; }

private:
	Dynamic2DVector<RoomData>			mRoomData;
	Dynamic2DVector<Entity>				mRoomEntities;
};