#pragma once

#include <EntityComponentSystem/World/World.h>

class MessageBroadcaster;

namespace PlayerEntity
{
	Entity Create(World& inWorld, MessageBroadcaster& inMsgBroadcaster);
}
