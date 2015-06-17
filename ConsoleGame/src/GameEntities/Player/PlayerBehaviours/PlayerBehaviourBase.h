#pragma once

#include <EntityComponentSystem/World/World.h>

#include <Maths/IVec2.h>
#include <GameEntities/Player/PlayerEnums.h>

class AttackMsg;

class PlayerBehaviourBase
{
public:
	virtual void OnStart(Entity inPlayer)											= 0;
	virtual void OnRestart(Entity inPlayer)											= 0;
	virtual void Update(Entity inPlayer, float inFrameTime)							= 0;
	virtual void OnFinish(Entity inPlayer)											= 0;
	virtual bool IsFinished() const													= 0;

	virtual bool OnAttacked(Entity inPlayer, const AttackMsg& inAttackMsg);
	virtual bool CanMoveToPosition(Entity inPlayer, const IVec2& inPosition) const;

};

class PlayerIdleBehaviour : public PlayerBehaviourBase
{
public:
	virtual void OnStart(Entity inPlayer);
	virtual void OnRestart(Entity inPlayer);
	virtual void Update(Entity inPlayer, float inFrameTime);
	virtual void OnFinish(Entity inPlayer);
	virtual bool IsFinished() const;

};
