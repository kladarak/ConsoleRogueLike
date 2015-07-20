#pragma once

#include <Maths/IVec2.h>
#include <EntityComponentSystem/World/World.h>

class MessageBroadcaster;

namespace CaterpillarMonster
{
	Entity Create(World& inWorld, MessageBroadcaster& inMsgBroadcaster, const IVec2& inPos);
}
