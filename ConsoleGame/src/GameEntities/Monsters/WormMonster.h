#pragma once

#include <EntityComponentSystem/World/World.h>

class MessageBroadcaster;
struct IVec2;

namespace WormMonster
{
	Entity Create(World& inWorld, MessageBroadcaster& inMsgBroadcaster, const IVec2& inPosition);
}
