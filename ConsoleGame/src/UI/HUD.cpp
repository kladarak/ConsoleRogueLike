#include "HUD.h"

#include <string>

#include <Core/Messaging/MessageBroadcaster.h>

#include <EntityComponent/Components/HealthComponent.h>
#include <EntityComponent/Components/PlayerComponent.h>
#include <Inventory/Items/ItemBase.h>

#include <Messages/Messages.h>

#include <Renderer/RenderTargetWriter.h>

#include "ScreenConstants.h"
#include "GameData.h"

namespace
{
	static const int kTopBarHeight		= 2;
	static const int kBottomBarHeight	= 1;
}

HUD::HUD()
	: mMessageBroadcaster	(nullptr)
	, mGameData				(nullptr)
	, mMoneyCollected		(0)
{
}

void HUD::Init(MessageBroadcaster& inMessageBroadcaster, GameData* inGameData)
{
	mGameData = inGameData;

	inMessageBroadcaster.Register<CoinCollectedMessage>(	[this] (const CoinCollectedMessage&)	{ OnCoinCollected();	} );
	inMessageBroadcaster.Register<PlayerIsDeadMsg>(			[this] (const PlayerIsDeadMsg&)			{ OnPlayerIsDead();		} );

	mHealthBar.Init(mGameData->mPlayer);
	mEquippedBar.Init(&mGameData->mPlayerData);

	mMessageBroadcaster = &inMessageBroadcaster;
}

void HUD::Update(float inFrameTime)
{
	if (mGameData->mPlayer.GetComponent<HealthComponent>()->IsDead())
	{
		mYouAreDeadDisplay.UpdateAnimation(inFrameTime);

		if (mYouAreDeadDisplay.IsAnimationFinished())
		{
			mMessageBroadcaster->Broadcast( BackToStartMenuMsg() );
		}
	}
}

RenderTarget HUD::GetTopBarRenderTarget() const
{
	RenderTargetWriter renderTargetWriter(100, kTopBarHeight);
	
	renderTargetWriter.Write( mHealthBar.GetRenderTarget(), 0, 0 );

	int equippedBarX = ScreenConstants::EMapCols - 13;
	renderTargetWriter.Write( mEquippedBar.GetRenderTarget(), equippedBarX, 0 );

	return renderTargetWriter.GetRenderTarget();
}

RenderTarget HUD::GetBottomBarRenderTarget() const
{
	RenderTargetWriter renderTargetWriter(100, kBottomBarHeight);
	
	std::string money = "Money: $" + std::to_string(mMoneyCollected) + "\n";
	renderTargetWriter.Write(money, 0, 0);

	return renderTargetWriter.GetRenderTarget();
}

RenderTarget HUD::GetOverlayRenderTarget() const
{
	if (mGameData->mPlayer.GetComponent<HealthComponent>()->IsDead())
	{
		return mYouAreDeadDisplay.GetRenderTarget();
	}

	return RenderTarget(0, 0);
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
