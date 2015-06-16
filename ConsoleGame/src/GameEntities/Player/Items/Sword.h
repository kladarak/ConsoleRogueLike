#include "ItemBase.h"

class Sword : public ItemBase
{
public:
	Sword();
	
	virtual void OnStartUsing(Entity inPlayer);
	virtual bool UpdateUsing(Entity inPlayer, float inFrameTime);
	virtual void OnStoppedUsing(Entity inPlayer);

private:
	float mSwipeTimeElapsed;

};
