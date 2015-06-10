#pragma once

#include <Maths/IVec2.h>

class World;
class MessageBroadcaster;

namespace CoinEntity
{
	void Create(World& inWorld, MessageBroadcaster& inMessageBroadcaster, const IVec2& inPos);
}
