#pragma once

#include <EntityComponentSystem/World/World.h>

#include "HealthBarUI.h"
#include "EquippedBarUI.h"

class MessageBroadcaster;

class HUD
{
public:
	HUD();

	void			Init(MessageBroadcaster& inMessageBroadcaster, Entity inPlayer);

	std::string		GetTopBarRenderBuffer() const;
	std::string		GetBottomBarRenderBuffer() const;

private:
	void			OnCoinCollected();
	
	HealthBarUI		mHealthBar;
	EquippedBarUI	mEquippedBar;

	Entity			mPlayer;
	int				mMoneyCollected;
};
