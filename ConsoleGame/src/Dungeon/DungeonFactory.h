#pragma once

#include <EntityComponentSystem/World/World.h>

class MessageBroadcaster;

namespace DungeonFactory
{
	std::vector<Entity> Generate(World& inGlobalSystems, MessageBroadcaster& inMessageBroadcaster);
}
