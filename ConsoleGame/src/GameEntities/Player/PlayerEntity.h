#pragma once

#include <EntityComponentSystem/World/World.h>

class MessageBroadcaster;
class GameData;

namespace PlayerEntity
{
	Entity Create(World& inWorld, MessageBroadcaster& inMsgBroadcaster, GameData* inGameData);
}
