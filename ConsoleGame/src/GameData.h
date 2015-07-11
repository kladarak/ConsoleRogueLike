#pragma once

#include <Dungeon/DungeonMap.h>
#include <GameEntities/Player/PlayerData.h>

class GameData
{
public:
	GameData() : mCurrentLevel(1) { }

	PlayerData	mPlayerData;
	DungeonMap	mDungeonMap;
	int			mCurrentLevel;
};
