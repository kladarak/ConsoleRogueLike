#pragma once

#include "ItemBase.h"

class BowPlayerBehaviour : public PlayerBehaviourBase
{
public:
	BowPlayerBehaviour();
	virtual void	OnStart(Entity inPlayer);
	virtual void	OnRestart(Entity inPlayer);
	virtual void	Update(Entity inPlayer, float inFrameTime);
	virtual void	OnFinish(Entity inPlayer);
	virtual bool	IsFinished() const;

	int				GetArrowCount() const { return mArrowCount; }

private:
	float			mAnimTimeElapsed;
	int				mArrowCount;
};

class Bow : public ItemBase
{
public:
	Bow();
	
	virtual PlayerBehaviourBase*	GetPlayerBehaviour() { return &mBehaviour; }
	virtual int						GetAmmoCount() const { return mBehaviour.GetArrowCount(); }

private:
	BowPlayerBehaviour				mBehaviour;

};
