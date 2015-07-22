#pragma once

#include <stdint.h>
#include <EntityComponentSystem/World/World.h>

class	MessageBroadcaster;
class	GameData;
struct	IVec2;

typedef Entity (*MonsterFactoryFunc)(World&, MessageBroadcaster&, const IVec2&);
typedef Entity (*ItemFactoryFunc)(World&, GameData*);

struct MonsterSpawnInfo
{
	int					mMonsterCount;
	MonsterFactoryFunc	mFactoryFunc;
};

struct LevelData
{
	int								mRoomCount;
	int								mMoneyCount;
	bool							mAddHeartContainer;
	ItemFactoryFunc					mItemFactoryFunc;
	std::vector<MonsterSpawnInfo>	mMonsterSpawnInfo;
};

LevelData gGenerateLevelData(uint32_t inLevelIndex);

