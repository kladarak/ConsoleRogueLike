#pragma once

#include <stdint.h>
#include <EntityComponentSystem/World/World.h>

class	MessageBroadcaster;
struct	IVec2;

struct MonsterSpawnInfo
{
	typedef Entity (*MonsterFactoryFunc)(World&, MessageBroadcaster&, const IVec2&);

	int					mMonsterCount;
	MonsterFactoryFunc	mFactoryFunc;
};

struct LevelData
{
	int								mRoomCount;
	int								mMoneyCount;
	std::vector<MonsterSpawnInfo>	mMonsterSpawnInfo;
};

extern const LevelData	kLevelData[];
extern const uint32_t	kLevelDataCount;
