#pragma once

#include <Core/Messaging/MessageBroadcaster.h>

#include "EGameState.h"

class StateMachine;
class GameData;

class StateMachineMessageHandler
{
public:
	StateMachineMessageHandler(StateMachine& inStateMachine, GameData& inGameData);

	StateMachineMessageHandler& operator=(StateMachineMessageHandler& inOther) = delete;

	void				ProcessStateChangeRequests();

private:
	void				PushState(EGameState inState);

	StateMachine&		mStateMachine;
	GameData&			mGameData;
	MessageBroadcaster	mMessageBroadcaster;

	enum ERequestType
	{
		ERequestType_GoTo,
		ERequestType_Push,
		ERequestType_Pop,
	};

	struct StateChangeRequest
	{
		ERequestType	mType;
		EGameState		mState;

		StateChangeRequest(ERequestType inType, EGameState inState);
	};

	std::vector<StateChangeRequest> mRequests;
};
