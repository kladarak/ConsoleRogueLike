#pragma once

#include <EntityComponentSystem/World/World.h>

#include "DungeonMap.h"

class MessageBroadcaster;
class GameData;
struct LevelData;

namespace DungeonFactory
{
	DungeonMap Generate(World& inGlobalSystems, MessageBroadcaster& inMessageBroadcaster, GameData* inGameData, const LevelData& inLevelData);
}
