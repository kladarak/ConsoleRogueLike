#include "HUD.h"

#include <string>

#include <Core/Messaging/MessageBroadcaster.h>

#include <EntityComponent/Components/PlayerComponent.h>
#include <GameEntities/Player/Items/ItemBase.h>

#include <Messages/Messages.h>

#include <Renderer/RenderTargetWriter.h>
#include "ScreenConstants.h"


HUD::HUD()
	: mMoneyCollected(0)
{
}

void HUD::Init(MessageBroadcaster& inMessageBroadcaster, Entity inPlayer)
{
	mPlayer = inPlayer;
	inMessageBroadcaster.Register<CoinCollectedMessage>( [this] (const CoinCollectedMessage&) { OnCoinCollected(); } );

	mHealthBar.Init(mPlayer);
	mEquippedBar.Init(mPlayer);
}

void HUD::OnCoinCollected()
{
	mMoneyCollected += 100;
}

std::string HUD::GetTopBarRenderBuffer() const
{
	RenderTargetWriter renderTargetWriter(100, 2);
	
	renderTargetWriter.Write( mHealthBar.GetRenderBuffer(), 0, 0 );

	int equippedBarX = ScreenConstants::EMapCols - 10;
	renderTargetWriter.Write( mEquippedBar.GetRenderBuffer(), equippedBarX, 0 );

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
