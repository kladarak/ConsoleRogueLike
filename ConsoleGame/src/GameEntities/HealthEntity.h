#pragma once

#include <Maths/IVec2.h>

class World;
class MessageBroadcaster;

namespace HealthEntity
{
	void Create(World& inWorld, MessageBroadcaster& inMsgBroadcaster, const IVec2& inPosition);
}
