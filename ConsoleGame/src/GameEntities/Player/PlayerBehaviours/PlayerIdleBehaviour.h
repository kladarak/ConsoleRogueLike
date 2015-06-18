#pragma once

#include "PlayerBehaviourBase.h"

class PlayerIdleBehaviour : public PlayerBehaviourBase
{
public:
	virtual void OnStart(Entity inPlayer);
	virtual void OnRestart(Entity inPlayer);
	virtual void Update(Entity inPlayer, float inFrameTime);
	virtual void OnFinish(Entity inPlayer);
	virtual bool IsFinished() const;

};
