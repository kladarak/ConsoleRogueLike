#pragma once

#include "ItemBase.h"

class BoomerangPlayerBehaviour : public PlayerBehaviourBase
{
public:
	BoomerangPlayerBehaviour();
	virtual void	OnStart(Entity inPlayer);
	virtual void	OnRestart(Entity inPlayer);
	virtual void	Update(Entity inPlayer, float inFrameTime);
	virtual void	OnFinish(Entity inPlayer);
	virtual bool	IsFinished() const;

	void			OnBoomerangReturned();

private:
	bool			mBoomerangIsInFlight;
};

class Boomerang : public ItemBase
{
public:
	Boomerang();

	virtual PlayerBehaviourBase*	GetPlayerBehaviour() { return &mBehaviour; }
	
private:
	BoomerangPlayerBehaviour		mBehaviour;

};
