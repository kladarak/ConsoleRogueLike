#pragma once

#include <stdint.h>

#include <StateMachine/StateBase.h>

class InventoryScreenState : public StateBase
{
public:
	InventoryScreenState(MessageBroadcaster* inStateMachineMsgBroadcaster, GameData* inGameData);

	virtual void			Update(float inFrameTime, const InputBuffer& inInput);
	virtual RenderTarget	GetRenderTarget() const;

private:
	int32_t					mHighlightedItem;
};
