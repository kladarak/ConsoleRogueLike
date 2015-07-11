#include "PlayerEntity.h"

#include <Core/Messaging/MessageBroadcaster.h>

#include <EntityComponentSystem/World/World.h>

#include <EntityComponent/Components/AnimationComponent.h>
#include <EntityComponent/Components/CollisionComponent.h>
#include <EntityComponent/Components/HealthComponent.h>
#include <EntityComponent/Components/InputHandlerComponent.h>
#include <EntityComponent/Components/MessageReceiverComponent.h>
#include <EntityComponent/Components/OrientationComponent.h>
#include <EntityComponent/Components/PlayerComponent.h>
#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/ProgramComponent.h>
#include <EntityComponent/Components/TriggerBoxComponent.h>
#include <EntityComponent/Components/RenderableComponent.h>

#include <Dungeon/DungeonMap.h>
#include <Messages/Messages.h>

#include "GameData.h"

namespace PlayerEntity
{

Entity Create(World& inWorld, MessageBroadcaster& inMsgBroadcaster, GameData* inGameData)
{
	Entity entity = inWorld.CreateEntity();

	IVec2 roomPosition(-1, -1);

	while (roomPosition.mX < 0)
	{
		size_t randomX = rand() % inGameData->mDungeonMap.GetColCount();
		size_t randomY = rand() % inGameData->mDungeonMap.GetRowCount();

		Entity room = inGameData->mDungeonMap.Get(randomX, randomY);
		if (room.IsValid())
		{
			roomPosition = room.GetComponent<PositionComponent>()->GetPosition();
		}
	}
	
	entity.AddComponent<AnimationComponent>();
	entity.AddComponent<CollisionComponent>();
	entity.AddComponent<HealthComponent>(3);
	entity.AddComponent<PlayerComponent>( &inGameData->mPlayerData );
	entity.AddComponent<OrientationComponent>( EOrientation_FaceDown );
	entity.AddComponent<PositionComponent>( roomPosition + IVec2(10, 10) );
	entity.AddComponent<RenderableComponent>();
	entity.AddComponent<TriggererComponent>();
	
	entity.AddComponent<InputHandlerComponent>()->RegisterHandler
	(
		[] (const Entity& inPlayer, const InputBuffer& inBuffer)
		{
			inPlayer.GetComponent<PlayerComponent>()->HandleInput(inBuffer);
		}
	);

	entity.AddComponent<ProgramComponent>()->RegisterProgram
	(
		[] (const Entity& inPlayer, float inFrameTime)
		{
			inPlayer.GetComponent<PlayerComponent>()->Update(inPlayer, inFrameTime);
		}
	);

	entity.AddComponent<MessageReceiverComponent>()->Register<AttackMsg>
	(
		[entity, &inMsgBroadcaster] (const AttackMsg& inAttackMsg)
		{
			entity.GetComponent<PlayerComponent>()->OnAttacked(entity, inAttackMsg, inMsgBroadcaster);
		}
	);

	return entity;
}

}
