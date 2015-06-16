#include "HUD.h"

#include <string>

#include <Core/Messaging/MessageBroadcaster.h>

#include <EntityComponent/Components/HealthComponent.h>
#include <EntityComponent/Components/PlayerComponent.h>
#include <GameEntities/Player/Items/ItemBase.h>

#include <Messages/Messages.h>

#include <Renderer/RenderTargetWriter.h>
#include "ScreenConstants.h"

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

	{
		auto playerComp = mPlayer.GetComponent<PlayerComponent>();
		auto item0 = playerComp->GetItemInSlot( Player::EItemSlot_Slot0 );
		auto item1 = playerComp->GetItemInSlot( Player::EItemSlot_Slot1 );

		int left = ScreenConstants::EMapCols - 10;
		renderTargetWriter.Write("Spc:",	left, 0);
		renderTargetWriter.Write("E:",		left, 1);

		if (nullptr != item0)
		{
			renderTargetWriter.Write(item0->GetHUDIcon(), left+5, 0);
		}

		if (nullptr != item1)
		{
			renderTargetWriter.Write(item1->GetHUDIcon(), left+5, 1);
		}
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
