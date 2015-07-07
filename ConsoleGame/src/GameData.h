#pragma once

#include <GameEntities/Player/PlayerData.h>

class GameData
{
public:
	GameData() : mCurrentLevel(1) { }

	PlayerData	mPlayerData;
	int			mCurrentLevel;
};
