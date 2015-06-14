#pragma once

#include <EntityComponentSystem/World/World.h>
#include <Maths/IVec2.h>

#include <EntityComponent/Components/MessageReceiverComponent.h>
#include <EntityComponent/Systems/PositionSystem.h>

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


namespace MessageHelpers
{

template<typename TMessage>
void BroadcastMessageToEntitiesAtPosition(World& inWorld, Entity inExceptThis, const IVec2& inPos, const TMessage& inMsg)
{
	auto attackedEntities = PositionSystem::GetListOfEntitiesAtPosition(inWorld, inExceptThis, inPos);
	for (auto entity : attackedEntities)
	{
		auto msgRecComp = entity.GetComponent<MessageReceiverComponent>();
		if (nullptr != msgRecComp)
		{
			msgRecComp->Broadcast( inMsg );
		}
	}
}

}
