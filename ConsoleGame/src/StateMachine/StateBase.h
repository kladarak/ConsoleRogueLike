#pragma once

#include <string>

#include <Renderer/RenderTarget.h>
#include "EGameState.h"

class InputBuffer;
class MessageBroadcaster;
class GameData;

class StateBase
{
public:
	StateBase(MessageBroadcaster* inStateMachineMsgBroadcaster, GameData* inGameData);
	virtual ~StateBase() { }

	virtual void			Update(float inFrameTime, const InputBuffer& inInput)	= 0;
	virtual	RenderTarget	GetRenderTarget() const									= 0;

protected:
	void					RequestGoToState(EGameState inState);
	void					RequestPushState(EGameState inState);
	void					RequestPopState();
	
	GameData*				mGameData;

private:
	MessageBroadcaster*		mStateMachineMsgBroadcaster;
};
