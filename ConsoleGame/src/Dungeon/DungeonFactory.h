#pragma once

#include <EntityComponentSystem/World/World.h>

#include "DungeonMap.h"

class MessageBroadcaster;

namespace DungeonFactory
{
	DungeonMap Generate(World& inGlobalSystems, MessageBroadcaster& inMessageBroadcaster);
}
