#pragma once

#include <stdint.h>

class DungeonMap;
class World;
class GameData;

namespace DungeonLockedDoorsAndKeysPlacer
{
	void sPlaceThem(const DungeonMap& inDungeon, World& inWorld, GameData* inGameData, uint32_t inMaxLockedDoors);
}
