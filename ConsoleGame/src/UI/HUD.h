#pragma once

#include <EntityComponentSystem/World/World.h>

#include "HealthBarUI.h"
#include "EquippedBarUI.h"
#include "YouAreDeadDisplay.h"

class MessageBroadcaster;

class HUD
{
public:
	HUD();

	void				Init(MessageBroadcaster& inMessageBroadcaster, Entity inPlayer);
	
	void				Update(float inFrameTime);

	std::string			GetTopBarRenderBuffer() const;
	std::string			GetBottomBarRenderBuffer() const;
	std::string			GetOverlayBuffer() const;

	int					GetTopBarHeight() const;
	int					GetBottomBarHeight() const;

private:
	void				OnCoinCollected();
	void				OnPlayerIsDead();
	
	MessageBroadcaster* mMessageBroadcaster;

	HealthBarUI			mHealthBar;
	EquippedBarUI		mEquippedBar;
	YouAreDeadDisplay	mYouAreDeadDisplay;

	Entity				mPlayer;
	int					mMoneyCollected;
};
