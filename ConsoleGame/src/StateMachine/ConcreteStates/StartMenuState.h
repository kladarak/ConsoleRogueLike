#pragma once

#include <stdint.h>
#include <StateMachine/StateBase.h>

class StartMenuState : public StateBase
{
public:
	StartMenuState(MessageBroadcaster* inStateMachineMsgBroadcaster, GameData* inGameData);
	virtual ~StartMenuState() { }

	virtual void			Update(float inFrameTime, const InputBuffer& inInput);
	virtual	RenderTarget	GetRenderTarget() const;

private:
	uint32_t				mHighlightedMenuOption;
};
