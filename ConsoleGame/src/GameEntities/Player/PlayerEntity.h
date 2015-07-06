#pragma once

#include <EntityComponentSystem/World/World.h>
#include <Dungeon/DungeonMap.h>

class MessageBroadcaster;
class GameData;

namespace PlayerEntity
{
	Entity Create(World& inWorld, MessageBroadcaster& inMsgBroadcaster, const DungeonMap& inDungeonMap, GameData* inGameData);
}
