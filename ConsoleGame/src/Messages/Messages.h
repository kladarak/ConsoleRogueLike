#pragma once

#include <EntityComponentSystem/World/World.h>
#include <Maths/IVec2.h>

class CoinCollectedMessage { };

class TouchedMonsterMsg
{
public:
	TouchedMonsterMsg(Entity inMonster, Entity inPlayer) 
		: mMonster(inMonster), mPlayer(inPlayer) { }

	Entity mMonster;
	Entity mPlayer;
};

class PlayerAttackMsg
{
public:
	PlayerAttackMsg(Entity inPlayer, const IVec2& inAttackPosition)
		: mPlayer(inPlayer), mAttackPosition(inAttackPosition) { }

	Entity	mPlayer;
	IVec2	mAttackPosition;
};