#pragma once

#include <EntityComponentSystem/World/World.h>

#include "HealthBarUI.h"
#include "EquippedBarUI.h"
#include "YouAreDeadDisplay.h"

class MessageBroadcaster;
class GameData;

class HUD
{
public:
	HUD();

	void				Init(MessageBroadcaster& inMessageBroadcaster, GameData* inGameData);
	
	void				Update(float inFrameTime);

	RenderTarget		GetTopBarRenderTarget() const;
	RenderTarget		GetBottomBarRenderTarget() const;
	RenderTarget		GetOverlayRenderTarget() const;

	int					GetTopBarHeight() const;
	int					GetBottomBarHeight() const;

private:
	void				OnCoinCollected();
	void				OnPlayerIsDead();
	
	MessageBroadcaster* mMessageBroadcaster;
	GameData*			mGameData;

	HealthBarUI			mHealthBar;
	EquippedBarUI		mEquippedBar;
	YouAreDeadDisplay	mYouAreDeadDisplay;

	int					mMoneyCollected;
};
