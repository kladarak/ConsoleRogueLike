#pragma once

class Entity;
class MessageBroadcaster;
class TouchedMonsterMsg;

namespace Player
{
	void UpdatePlayer(const Entity& inPlayer, float inFrameTime, MessageBroadcaster& inMsgBroadcaster);
	void OnTouchedMonster(const TouchedMonsterMsg& inMsg);
}
