#pragma once

class World;
class MessageBroadcaster;
class GameData;

namespace ItemDropHandler
{
	void Register(World& inWorld, MessageBroadcaster& inMsgBroadcaster, GameData* inGameData);
}
