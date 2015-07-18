#pragma once

#include <EntityComponentSystem/World/World.h>

class	MessageBroadcaster;
class	GameData;
struct	IVec2;

namespace ArcherMonster
{
	Entity Create(World& inWorld, MessageBroadcaster& inMsgBroadcaster, const IVec2& inPosition);
}