#pragma once

#include "ItemBase.h"

class Bow : public ItemBase
{
public:
	Bow();
	
	virtual void OnStartUsing(Entity inPlayer);
	virtual bool UpdateUsing(Entity inPlayer, float inFrameTime);
	virtual void OnStoppedUsing(Entity inPlayer);

private:
	float mAnimTimeElapsed;

};
