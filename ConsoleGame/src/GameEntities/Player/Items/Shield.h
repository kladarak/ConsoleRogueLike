#pragma once

#include "ItemBase.h"

class Shield : public ItemBase
{
public:
	Shield();
	
	virtual void OnStartUsing(Entity inPlayer);
	virtual bool UpdateUsing(Entity inPlayer, float inFrameTime);
	virtual void OnStoppedUsing(Entity inPlayer);

private:
	bool mHeldUp;
};
