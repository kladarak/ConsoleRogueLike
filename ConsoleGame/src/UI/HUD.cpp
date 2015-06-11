#include "HUD.h"

#include <Core/Messaging/MessageBroadcaster.h>

#include <Messages/Messages.h>

HUD::HUD()
	: mMoneyCollected(0)
	, mDirty(true)
{
}

void HUD::Init(MessageBroadcaster& inMessageBroadcaster)
{
	inMessageBroadcaster.Register<CoinCollectedMessage>( [this] (const CoinCollectedMessage&) { OnCoinCollected(); } );
}

void HUD::OnCoinCollected()
{
	mMoneyCollected += 100;
	mDirty = true;
}

void HUD::Render()
{
	printf("Money: $%i", mMoneyCollected);

	mDirty = false;
}
