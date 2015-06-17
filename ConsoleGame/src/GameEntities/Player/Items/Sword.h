#include "ItemBase.h"


class SwordPlayerBehaviour : public PlayerBehaviourBase
{
public:
	SwordPlayerBehaviour() : mSwipeTimeElapsed(0.0f) { }
	virtual void OnStart(Entity inPlayer);
	virtual void OnRestart(Entity inPlayer);
	virtual void Update(Entity inPlayer, float inFrameTime);
	virtual void OnFinish(Entity inPlayer);
	virtual bool IsFinished() const;

private:
	float mSwipeTimeElapsed;
};

class Sword : public ItemBase
{
public:
	Sword();
	
	virtual PlayerBehaviourBase*	GetPlayerBehaviour() { return &mBehaviour; }

private:
	SwordPlayerBehaviour			mBehaviour;

};
