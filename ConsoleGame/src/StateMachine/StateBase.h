#pragma once

#include <string>

#include "EGameState.h"

class InputBuffer;
class MessageBroadcaster;

class StateBase
{
public:
	StateBase() : mStateMachineMsgBroadcaster(nullptr)	{ }
	virtual ~StateBase()								{ }
	
	void				Initialise(MessageBroadcaster* inStateMachineMsgBroadcaster);

	virtual void		Update(float inFrameTime, const InputBuffer& inInput)	= 0;
	virtual	std::string GetRenderBuffer() const									= 0;

protected:
	void				RequestGoToState(EGameState inState);
	void				RequestPushState(EGameState inState);
	void				RequestPopState();

private:
	virtual void		Init()													= 0;

	MessageBroadcaster* mStateMachineMsgBroadcaster;
};
