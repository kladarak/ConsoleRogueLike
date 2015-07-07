#pragma once

#include <StateMachine/StateBase.h>

class StartNewLevelIntroState : public StateBase
{
public:
	StartNewLevelIntroState(MessageBroadcaster* inStateMachineMsgBroadcaster, GameData* inGameData);
	
	virtual void		Update(float inFrameTime, const InputBuffer& inInput);
	virtual	std::string GetRenderBuffer() const;

private:
	float mAnimTime;
};
