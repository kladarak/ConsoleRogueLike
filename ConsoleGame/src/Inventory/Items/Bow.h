#pragma once

#include "ItemBase.h"

class BowPlayerBehaviour : public PlayerBehaviourBase
{
public:
	BowPlayerBehaviour() : mAnimTimeElapsed(0.0f) { }
	virtual void OnStart(Entity inPlayer);
	virtual void OnRestart(Entity inPlayer);
	virtual void Update(Entity inPlayer, float inFrameTime);
	virtual void OnFinish(Entity inPlayer);
	virtual bool IsFinished() const;

private:
	float mAnimTimeElapsed;
};

class Bow : public ItemBase
{
public:
	Bow();
	
	virtual PlayerBehaviourBase*	GetPlayerBehaviour() { return &mBehaviour; }

private:
	BowPlayerBehaviour				mBehaviour;

};
