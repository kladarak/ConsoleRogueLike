#pragma once

#include <EntityComponentSystem/World/World.h>
#include <Maths/IVec2.h>

class MessageBroadcaster;

namespace Stairs
{
	Entity Create(World& inWorld, MessageBroadcaster& inMsgBroadcaster, const IVec2& inPosition);
}
