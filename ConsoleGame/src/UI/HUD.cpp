#include "HUD.h"

#include <string>

#include <Core/Messaging/MessageBroadcaster.h>

#include <EntityComponent/Components/HealthComponent.h>

#include <Messages/Messages.h>

#include <Renderer/RenderTargetWriter.h>

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
	RenderTargetWriter renderTargetWriter(100, 2);
	
	{
		auto health = mPlayer.GetComponent<HealthComponent>();
		int current = health->GetCurrentHealth();
		int max		= health->GetMaxHealth();

		std::string healthBar(max, '.');
		for (int i = 0; i < current; ++i)
		{
			healthBar[i] = kHeartIcon;
		}
	
		renderTargetWriter.Write(healthBar, 0, 0);
	}




	return renderTargetWriter.GetRenderBuffer();
}

std::string HUD::GetBottomBarRenderBuffer() const
{
	RenderTargetWriter renderTargetWriter(100, 1);
	
	{
		std::string money = "Money: $" + std::to_string(mMoneyCollected) + "\n";
		renderTargetWriter.Write(money, 0, 0);
	}

	return renderTargetWriter.GetRenderBuffer();
}
