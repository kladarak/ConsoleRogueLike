#pragma once

#include <EntityComponentSystem/World/World.h>

#include <Maths/IVec2.h>
#include <Containers/ContainerMacros.h>

#include <EntityComponent/Components/MessageReceiverComponent.h>

#include <EntityComponent/Systems/PositionSystem.h>
#include <EntityComponent/Systems/CollisionSystem.h>

#include <StateMachine/EGameState.h>

class CoinCollectedMessage	{ };
class PlayerIsDeadMsg		{ };
class BackToStartMenuMsg	{ };
class PlayerWentDownStairs	{ };

class GoToStateMsg
{
public:
	GoToStateMsg(EGameState inState) : mTargetState(inState) { }
	operator EGameState () const { return mTargetState; }
	EGameState mTargetState;
};

class PushStateMsg
{
public:
	PushStateMsg(EGameState inState) : mTargetState(inState) { }
	operator EGameState () const { return mTargetState; }
	EGameState mTargetState;
};

class PopStateMsg { };


class AttackMsg
{
public:
	enum EEffect
	{
		EEffect_None,
		EEffect_PushBack,
	};

	AttackMsg(Entity inAttacker, const IVec2& inAttackPosition, const IVec2& inAttackDirection, EEffect inEffect)
		: mAttacker(inAttacker), mAttackPosition(inAttackPosition), mAttackDirection(inAttackDirection), mEffect(inEffect) { }

	Entity	mAttacker;
	IVec2	mAttackPosition;
	IVec2	mAttackDirection;
	EEffect	mEffect;
};

class UnlockDoorMsg
{
public:
	UnlockDoorMsg(const IVec2& inPosition) : mPosition(inPosition) { }
	IVec2 mPosition;
};

namespace MessageHelpers
{

template<typename TMessage>
void BroadcastMessageToEntitiesAtPosition(World& inWorld, Entity inExceptThis, const IVec2& inPos, const TMessage& inMsg)
{
	auto entitiesCollidedWith	= CollisionSystem::GetListofCollidablesAtPosition(inWorld, inPos);
	auto entitiesAtPosition		= PositionSystem::GetListOfEntitiesAtPosition(inWorld, inExceptThis, inPos);
	auto interestedEntities		= MergeSortedVectors(entitiesCollidedWith, entitiesAtPosition);

	for (auto entity : interestedEntities)
	{
		auto msgRecComp = entity.GetComponent<MessageReceiverComponent>();
		if (entity != inExceptThis && nullptr != msgRecComp)
		{
			msgRecComp->Broadcast( inMsg );
		}
	}
}

}
