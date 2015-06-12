#include "HUD.h"

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

void HUD::RenderTop() const
{
	auto health = mPlayer.GetComponent<HealthComponent>();
	int current = health->GetCurrentHealth();
	int max		= health->GetMaxHealth();

	std::string buffer(max, '.');
	for (int i = 0; i < current; ++i)
	{
		buffer[i] = kHeartIcon;
	}

	printf("%s\n", buffer.c_str());
}

void HUD::RenderBottom() const
{
	printf("Money: $%i\n", mMoneyCollected);
}
