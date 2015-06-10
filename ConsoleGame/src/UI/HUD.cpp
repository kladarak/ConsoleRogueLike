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
	inMessageBroadcaster.Register<CoinCollectedMessage, HUD, &HUD::OnCoinCollected>(this);
}

void HUD::OnCoinCollected(const CoinCollectedMessage&)
{
	mMoneyCollected += 100;
	mDirty = true;
}

void HUD::Render()
{
	printf("Money: $%i", mMoneyCollected);

	mDirty = false;
}
