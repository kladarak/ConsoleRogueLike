#pragma once

#include <Renderer/Colour.h>

#include "ItemBase.h"

class DoorKeyPlayerBehaviour : public PlayerBehaviourBase
{
public:
	DoorKeyPlayerBehaviour(EColour inKeyColour) : mKeyColour(inKeyColour), mAnimTimeElapsed(0.0f) { }
	virtual void OnStart(Entity inPlayer);
	virtual void OnRestart(Entity inPlayer);
	virtual void Update(Entity inPlayer, float inFrameTime);
	virtual void OnFinish(Entity inPlayer);
	virtual bool IsFinished() const;

private:
	float	mAnimTimeElapsed;
	EColour mKeyColour;
};

class DoorKey : public ItemBase
{
public:
	static const std::string kName;

	DoorKey(EColour inKeyColour);
	virtual PlayerBehaviourBase*	GetPlayerBehaviour() { return &mBehaviour; }

private:
	DoorKeyPlayerBehaviour			mBehaviour;
};

template< int TColour >
class ColouredDoorKey : public DoorKey
{
public:
	ColouredDoorKey() : DoorKey((EColour) TColour) { }
};
