#pragma once

#include <GameEntities/Player/PlayerData.h>

class GameData
{
public:
	GameData() : mCurrentLevel(0) { }

	PlayerData	mPlayerData;
	int			mCurrentLevel;
};
