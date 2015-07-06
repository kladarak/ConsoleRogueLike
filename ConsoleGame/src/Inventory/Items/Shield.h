#pragma once

#include "ItemBase.h"

class ShieldPlayerBehaviour : public PlayerBehaviourBase
{
public:
	ShieldPlayerBehaviour() : mHeldUp(false) { }
	virtual void OnStart(Entity inPlayer);
	virtual void OnRestart(Entity inPlayer);
	virtual void Update(Entity inPlayer, float inFrameTime);
	virtual void OnFinish(Entity inPlayer);
	virtual bool IsFinished() const;
	
	virtual bool OnAttacked(Entity inPlayer, const AttackMsg& inAttackMsg);
	virtual bool CanMoveToPosition(Entity inPlayer, const IVec2& inPosition) const;

private:
	bool mHeldUp;
};

class Shield : public ItemBase
{
public:
	Shield();
	
	virtual PlayerBehaviourBase*	GetPlayerBehaviour() { return &mBehaviour; }

private:
	ShieldPlayerBehaviour			mBehaviour;

};
