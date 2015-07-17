#pragma once

#include <StateMachine/StateBase.h>

class StartNewLevelIntroState : public StateBase
{
public:
	StartNewLevelIntroState(MessageBroadcaster* inStateMachineMsgBroadcaster, GameData* inGameData);
	
	virtual void			Update(float inFrameTime, const InputBuffer& inInput);
	virtual	RenderTarget	GetRenderTarget() const;

private:
	float					mAnimTime;
};
