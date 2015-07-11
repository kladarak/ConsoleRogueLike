#pragma once

#include <StateMachine/StateBase.h>

class MapScreenState : public StateBase
{
public:
	MapScreenState(MessageBroadcaster* inStateMachineMsgBroadcaster, GameData* inGameData);

	virtual void		Update(float inFrameTime, const InputBuffer& inInput);
	virtual std::string GetRenderBuffer() const;

private:

};
