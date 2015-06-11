#pragma once

class World;
class MessageBroadcaster;
struct IVec2;

namespace MonsterEntityFactory
{
	void Create(World& inWorld, MessageBroadcaster& inMsgBroadcaster, const IVec2& inPosition);
}
