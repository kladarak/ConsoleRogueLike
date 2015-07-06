#include "StateBase.h"

#include <Core/Messaging/MessageBroadcaster.h>
#include <Messages/Messages.h>

StateBase::StateBase(MessageBroadcaster* inStateMachineMsgBroadcaster, GameData* inGameData)
	: mGameData(inGameData)
	, mStateMachineMsgBroadcaster(inStateMachineMsgBroadcaster)
{
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
