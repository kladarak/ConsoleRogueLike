#include "DungeonGeneratorUtils.h"

#include <EntityComponentSystem/World/World.h>

#include <EntityComponent/Components/PositionComponent.h>

#include "DungeonMap.h"
#include "RoomEntity.h"

namespace DungeonGeneratorUtils
{

IVec2 sGetRandomValidRoomIndex(const DungeonMap& inDungeon)
{
	int col, row;

	do
	{
		col = rand() % inDungeon.GetColCount();
		row = rand() % inDungeon.GetRowCount();
	}
	while ( !inDungeon.GetRoomData().Get(col, row).mIsValid );

	return IVec2(col, row);
}

IVec2 sGetRandomValidRoomPosition(const DungeonMap& inDungeon)
{
	IVec2	roomIndex	= sGetRandomValidRoomIndex(inDungeon);
	auto	room		= inDungeon.GetRoomEntities().Get(roomIndex);
	return room.GetComponent<PositionComponent>()->GetPosition();
}

IVec2 sGetRandomOffsetInRoom(const IRect& inObjectBounds)
{
	// Generate random offset considering wall/door margins and item size.
	static const int kRoomMargin = 2;

	int validAreaWidth = ERoomDimensions_Width - (kRoomMargin*2 + inObjectBounds.mWidth);
	int validAreaHeight = ERoomDimensions_Height - (kRoomMargin*2 + inObjectBounds.mHeight);
	int x = rand() % validAreaWidth;
	int y = rand() % validAreaHeight;
	x += kRoomMargin + inObjectBounds.mX;
	y += kRoomMargin + inObjectBounds.mY;

	return IVec2(x, y);
}

}
