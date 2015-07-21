#pragma once

#include "ItemBase.h"

class MessageBroadcaster;
struct IVec2;

class PigRiderPlayerBehaviour : public PlayerBehaviourBase
{
public:
	PigRiderPlayerBehaviour() : mTimeSinceMovement(0.0f), mIsOnPig(false) { }
	virtual void OnStart(Entity inPlayer);
	virtual void OnRestart(Entity inPlayer);
	virtual void Update(Entity inPlayer, float inFrameTime);
	virtual void OnFinish(Entity inPlayer);
	virtual bool IsFinished() const;

private:
	float	mTimeSinceMovement;
	bool	mIsOnPig;
};

class Pig : public ItemBase
{
public:
	Pig();
	
	virtual PlayerBehaviourBase*	GetPlayerBehaviour() { return &mBehaviour; }

private:
	PigRiderPlayerBehaviour			mBehaviour;

};
