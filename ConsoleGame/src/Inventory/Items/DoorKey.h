#pragma once

#include "ItemBase.h"

class DoorKeyPlayerBehaviour : public PlayerBehaviourBase
{
public:
	DoorKeyPlayerBehaviour() : mAnimTimeElapsed(0.0f) { }
	virtual void OnStart(Entity inPlayer);
	virtual void OnRestart(Entity inPlayer);
	virtual void Update(Entity inPlayer, float inFrameTime);
	virtual void OnFinish(Entity inPlayer);
	virtual bool IsFinished() const;

private:
	float mAnimTimeElapsed;
};

class DoorKey : public ItemBase
{
public:
	static const std::string kName;

	DoorKey();
	virtual PlayerBehaviourBase*	GetPlayerBehaviour() { return &mBehaviour; }

private:
	DoorKeyPlayerBehaviour			mBehaviour;
};
