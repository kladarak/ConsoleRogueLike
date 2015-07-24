#pragma once

#include <Maths/IRect.h>

class DungeonMap;

namespace DungeonGeneratorUtils
{
	IVec2 sGetRandomValidRoomIndex(const DungeonMap& inDungeon);
	IVec2 sGetRandomValidRoomPosition(const DungeonMap& inDungeon);
	IVec2 sGetRandomOffsetInRoom(const IRect& inObjectBounds = IRect(0, 0, 1, 1));
}
