#include "HUD.h"

#include <string>

#include <Core/Messaging/MessageBroadcaster.h>

#include <EntityComponent/Components/HealthComponent.h>
#include <EntityComponent/Components/PlayerComponent.h>
#include <GameEntities/Player/Items/ItemBase.h>

#include <Messages/Messages.h>

#include <Renderer/RenderTargetWriter.h>
#include "ScreenConstants.h"

namespace
{
	static const int kTopBarHeight		= 2;
	static const int kBottomBarHeight	= 1;
}

HUD::HUD()
	: mMessageBroadcaster(nullptr)
	, mMoneyCollected(0)
{
}

void HUD::Init(MessageBroadcaster& inMessageBroadcaster, Entity inPlayer)
{
	mPlayer = inPlayer;
	inMessageBroadcaster.Register<CoinCollectedMessage>(	[this] (const CoinCollectedMessage&)	{ OnCoinCollected();	} );
	inMessageBroadcaster.Register<PlayerIsDeadMsg>(			[this] (const PlayerIsDeadMsg&)			{ OnPlayerIsDead();		} );

	mHealthBar.Init(mPlayer);
	mEquippedBar.Init(mPlayer);

	mMessageBroadcaster = &inMessageBroadcaster;
}

void HUD::Update(float inFrameTime)
{
	if (mPlayer.GetComponent<HealthComponent>()->IsDead())
	{
		mYouAreDeadDisplay.UpdateAnimation(inFrameTime);

		if (mYouAreDeadDisplay.IsAnimationFinished())
		{
			mMessageBroadcaster->Broadcast( BackToStartMenuMsg() );
		}
	}
}

std::string HUD::GetTopBarRenderBuffer() const
{
	RenderTargetWriter renderTargetWriter(100, kTopBarHeight);
	
	renderTargetWriter.Write( mHealthBar.GetRenderBuffer(), 0, 0 );

	int equippedBarX = ScreenConstants::EMapCols - 10;
	renderTargetWriter.Write( mEquippedBar.GetRenderBuffer(), equippedBarX, 0 );

	return renderTargetWriter.GetRenderBuffer();
}

std::string HUD::GetBottomBarRenderBuffer() const
{
	RenderTargetWriter renderTargetWriter(100, kBottomBarHeight);
	
	std::string money = "Money: $" + std::to_string(mMoneyCollected) + "\n";
	renderTargetWriter.Write(money, 0, 0);

	return renderTargetWriter.GetRenderBuffer();
}

std::string	HUD::GetOverlayBuffer() const
{
	if (mPlayer.GetComponent<HealthComponent>()->IsDead())
	{
		return mYouAreDeadDisplay.GetRenderBuffer();
	}

	return "";
}

int HUD::GetTopBarHeight() const
{
	return kTopBarHeight;
}

int HUD::GetBottomBarHeight() const
{
	return kBottomBarHeight;
}

void HUD::OnCoinCollected()
{
	mMoneyCollected += 100;
}

void HUD::OnPlayerIsDead()
{
	mYouAreDeadDisplay.ResetAnimation();
}
