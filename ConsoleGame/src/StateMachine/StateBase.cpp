#include "StateBase.h"

#include <Core/Messaging/MessageBroadcaster.h>
#include <Messages/Messages.h>

void StateBase::Initialise(MessageBroadcaster* inStateMachineMsgBroadcaster)
{
	mStateMachineMsgBroadcaster = inStateMachineMsgBroadcaster;
	Init();
}

void StateBase::RequestGoToState(EGameState inState)
{
	mStateMachineMsgBroadcaster->Broadcast( GoToStateMsg(inState) );
}

void StateBase::RequestPushState(EGameState inState)
{
	mStateMachineMsgBroadcaster->Broadcast( PushStateMsg(inState) );
}

void StateBase::RequestPopState()
{
	mStateMachineMsgBroadcaster->Broadcast( PopStateMsg() );
}
