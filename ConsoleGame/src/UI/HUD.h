#pragma once

class MessageBroadcaster;
class CoinCollectedMessage;

class HUD
{
public:
	HUD();

	void	Init(MessageBroadcaster& inMessageBroadcaster);

	bool	NeedsRefreshing() const { return mDirty; }
	void	Render();

private:
	void	OnCoinCollected();

	int		mMoneyCollected;
	bool	mDirty;
};