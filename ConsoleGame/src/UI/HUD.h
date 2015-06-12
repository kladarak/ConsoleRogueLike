#pragma once

#include <EntityComponentSystem/World/World.h>

class MessageBroadcaster;

class HUD
{
public:
	HUD();

	void	Init(MessageBroadcaster& inMessageBroadcaster, Entity inPlayer);

	void	RenderTop() const;
	void	RenderBottom() const;

private:
	void	OnCoinCollected();

	Entity	mPlayer;
	int		mMoneyCollected;
};
