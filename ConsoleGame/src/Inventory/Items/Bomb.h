#pragma once

#include "ItemBase.h"

class BombPlayerBehaviour : public PlayerBehaviourBase
{
public:
	BombPlayerBehaviour();
	virtual void	OnStart(Entity inPlayer);
	virtual void	OnRestart(Entity inPlayer);
	virtual void	Update(Entity inPlayer, float inFrameTime);
	virtual void	OnFinish(Entity inPlayer);
	virtual bool	IsFinished() const;

	int				GetBombCount() const	{ return mBombCount; }
	void			IncBombCount()			{ mBombCount++; }

private:
	int				mBombCount;
};

class Bomb : public ItemBase
{
public:
	Bomb();
	
	virtual int						GetAmmoCount() const								{ return mBehaviour.GetBombCount(); }
	virtual void					SpawnAmmo(World& inWorld, const IVec2& inPosition);

	virtual PlayerBehaviourBase*	GetPlayerBehaviour()								{ return &mBehaviour; }
	
private:
	BombPlayerBehaviour				mBehaviour;

};
