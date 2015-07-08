#pragma once

#include <EntityComponentSystem/World/World.h>

#include "DungeonMap.h"

class MessageBroadcaster;
class GameData;

namespace DungeonFactory
{
	DungeonMap Generate(World& inGlobalSystems, MessageBroadcaster& inMessageBroadcaster, GameData* inGameData);
}
