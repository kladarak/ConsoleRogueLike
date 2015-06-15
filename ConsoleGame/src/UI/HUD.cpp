#include "HUD.h"

#include <string>

#include <Core/Messaging/MessageBroadcaster.h>

#include <EntityComponent/Components/HealthComponent.h>

#include <Messages/Messages.h>

static const char kHeartIcon = 3;

HUD::HUD()
	: mMoneyCollected(0)
{
}

void HUD::Init(MessageBroadcaster& inMessageBroadcaster, Entity inPlayer)
{
	mPlayer = inPlayer;
	inMessageBroadcaster.Register<CoinCollectedMessage>( [this] (const CoinCollectedMessage&) { OnCoinCollected(); } );
}

void HUD::OnCoinCollected()
{
	mMoneyCollected += 100;
}

std::string HUD::GetTopBarRenderBuffer() const
{
	auto health = mPlayer.GetComponent<HealthComponent>();
	int current = health->GetCurrentHealth();
	int max		= health->GetMaxHealth();

	std::string buffer(max, '.');
	for (int i = 0; i < current; ++i)
	{
		buffer[i] = kHeartIcon;
	}

	return buffer + "\n";
}

std::string HUD::GetBottomBarRenderBuffer() const
{
	std::string out;

	out = "Money: $" + std::to_string(mMoneyCollected) + "\n";

	return out;
}
