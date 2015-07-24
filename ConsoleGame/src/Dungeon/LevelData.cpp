#include "LevelData.h"

#include <algorithm>

#include <Containers/ContainerMacros.h>

#include <GameEntities/Monsters/WormMonster.h>
#include <GameEntities/Monsters/WallSparkMonster.h>
#include <GameEntities/Monsters/ArcherMonster.h>
#include <GameEntities/Monsters/SpinnerEntity.h>
#include <GameEntities/Monsters/CaterpillarMonster.h>
#include <GameEntities/Monsters/GunTurretEntity.h>

#include <GameEntities/ItemEntity.h>

#include <Inventory/Items/Sword.h>
#include <Inventory/Items/Bow.h>
#include <Inventory/Items/Shield.h>
#include <Inventory/Items/Bomb.h>
#include <Inventory/Items/Pig.h>
#include <Inventory/Items/Boomerang.h>


static const ItemFactoryFunc kItemFactoryFunctions[] =
{
	&ItemEntity::Create< Sword >,
	&ItemEntity::Create< Shield >,
	&ItemEntity::Create< Bomb >,
	&ItemEntity::Create< Boomerang >,
	&ItemEntity::Create< Bow >,
	&ItemEntity::Create< Pig >,
};

static const MonsterFactoryFunc kMonsterFactoryFunctions[] =
{
	&WormMonster::Create,
	&SpinnerEntity::Create,
	&CaterpillarMonster::Create,
	&WallSparkMonster::Create,
	&ArcherMonster::Create,
	&GunTurretEntity::Create,
};

LevelData gGenerateLevelData(uint32_t inLevelIndex)
{
	LevelData data;

	// Room count will equal the level index.
	data.mRoomCount		= inLevelIndex;

	// Money count will be double the room count.
	data.mMoneyCount	= data.mRoomCount * 2;

	data.mLockedDoorsCount = std::min(data.mRoomCount / 5, 4);

	// Every other level, place a new item into the dungeon.
	if ((inLevelIndex % 2) == 1 && ((inLevelIndex / 2) < gElemCount(kItemFactoryFunctions)))
	{
		data.mItemFactoryFunc = kItemFactoryFunctions[ inLevelIndex / 2 ];
	}
	else
	{
		data.mItemFactoryFunc = nullptr;
	}

	data.mAddHeartContainer = ((inLevelIndex % 2) == 0) && (inLevelIndex <= (17*2));
	
	{
		// Create random monster list, whose count and types are based on level index.

		int maxMonsterTypeIndex = std::min((inLevelIndex / 2) + 1, gElemCount(kMonsterFactoryFunctions));
		int avgMonstersPerRoom	= (inLevelIndex / 4) + 1;
		int totalMonstersCount	= avgMonstersPerRoom * data.mRoomCount;

		for (int i = 0; i < maxMonsterTypeIndex; ++i)
		{
			MonsterSpawnInfo spawnInfo;
			spawnInfo.mMonsterCount = 0;
			spawnInfo.mFactoryFunc	= kMonsterFactoryFunctions[i];
			data.mMonsterSpawnInfo.push_back(spawnInfo);
		}
	
		for (int i = 0; i < totalMonstersCount; ++i)
		{
			int monsterIndex = rand() % data.mMonsterSpawnInfo.size();
			data.mMonsterSpawnInfo[monsterIndex].mMonsterCount++;
		}
	}

	return data;
}
