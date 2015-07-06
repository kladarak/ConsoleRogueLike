#include "StateMachineMessageHandler.h"

#include <Messages/Messages.h>

#include "ConcreteStates/StartMenuState.h"
#include "ConcreteStates/InGameState.h"
#include "ConcreteStates/InventoryScreenState.h"

#include "StateMachine.h"

namespace
{
	template<typename TState>
	void InitAndPush(StateMachine& inStateMachine, MessageBroadcaster* inBroadcaster, GameData* inGameData)
	{
		inStateMachine.PushState<TState>(inBroadcaster, inGameData);
	};
};

StateMachineMessageHandler::StateChangeRequest::StateChangeRequest(ERequestType inType, EGameState inState)
	: mType		(inType)
	, mState	(inState)
{
}

StateMachineMessageHandler::StateMachineMessageHandler(StateMachine& inStateMachine, GameData& inGameData)
	: mStateMachine(inStateMachine)
	, mGameData(inGameData)
{
	mMessageBroadcaster.Register<GoToStateMsg>( [&] (const GoToStateMsg& inGoToState)
	{
		mRequests.emplace_back(ERequestType_GoTo, inGoToState);
	} );

	mMessageBroadcaster.Register<PushStateMsg>( [&] (const PushStateMsg& inPushState)
	{
		mRequests.emplace_back(ERequestType_Push, inPushState);
	} );

	mMessageBroadcaster.Register<PopStateMsg>( [&] (const PopStateMsg&)
	{
		mRequests.emplace_back(ERequestType_Pop, EGameState_None);
	} );

	mMessageBroadcaster.Broadcast( GoToStateMsg(EGameState_StartMenu) );
}

void StateMachineMessageHandler::ProcessStateChangeRequests()
{
	auto stateRequestCopy = mRequests;
	mRequests.clear();

	for (auto request : stateRequestCopy)
	{
		switch (request.mType)
		{
			case ERequestType_GoTo:
				mStateMachine.Clear();
				// Fall through case

			case ERequestType_Push:
				PushState(request.mState);
				break;

			case ERequestType_Pop:
				mStateMachine.PopState();
				break;

			default:
				break;
		}
	}
}

void StateMachineMessageHandler::PushState(EGameState inState)
{
	switch (inState)
	{
		case EGameState_StartMenu:		InitAndPush<StartMenuState>			(mStateMachine,	&mMessageBroadcaster, &mGameData);	break;
		case EGameState_InGame:			InitAndPush<InGameState>			(mStateMachine,	&mMessageBroadcaster, &mGameData);	break;
		case EGameState_InventoryView:	InitAndPush<InventoryScreenState>	(mStateMachine,	&mMessageBroadcaster, &mGameData);	break;
		default: break;
	}
}
