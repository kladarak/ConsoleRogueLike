#pragma once

#include <EntityComponentSystem/World/World.h>

class MessageBroadcaster;

class HUD
{
public:
	HUD();

	void		Init(MessageBroadcaster& inMessageBroadcaster, Entity inPlayer);

	std::string	GetTopBarRenderBuffer() const;
	std::string	GetBottomBarRenderBuffer() const;

private:
	void		OnCoinCollected();

	Entity		mPlayer;
	int			mMoneyCollected;
};
