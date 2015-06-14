#pragma once

#include <EntityComponentSystem/World/World.h>
#include <Maths/IVec2.h>

class CoinCollectedMessage { };

class AttackMsg
{
public:
	AttackMsg(Entity inAttacker, const IVec2& inAttackPosition, const IVec2& inAttackDirection)
		: mAttacker(inAttacker), mAttackPosition(inAttackPosition), mAttackDirection(inAttackDirection) { }

	Entity	mAttacker;
	IVec2	mAttackPosition;
	IVec2	mAttackDirection;
};
