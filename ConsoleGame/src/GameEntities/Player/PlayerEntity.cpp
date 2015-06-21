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

#include <Messages/Messages.h>

#include "Items/Sword.h"
#include "Items/Bow.h"
#include "Items/Shield.h"
#include "Items/DoorKey.h"

namespace PlayerEntity
{

Entity Create(World& inWorld, MessageBroadcaster& inMsgBroadcaster)
{
	Entity entity = inWorld.CreateEntity();
	
	entity.AddComponent<AnimationComponent>();
	entity.AddComponent<CollisionComponent>();
	entity.AddComponent<HealthComponent>(3);
	entity.AddComponent<PlayerComponent>();
	entity.AddComponent<OrientationComponent>( EOrientation_FaceDown );
	entity.AddComponent<PositionComponent>(10, 10);
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
	
	{
		auto playerComp = entity.GetComponent<PlayerComponent>();
		auto& inventory = playerComp->GetInventory();

		auto sword	= new Sword();
		auto bow	= new Bow();
		inventory.AddItem( sword );
		inventory.AddItem( bow );
		inventory.AddItem( new Shield() );
		inventory.AddItem( new DoorKey() );

		playerComp->SetItemInSlot( sword,	Player::EItemSlot_Slot0 );
		playerComp->SetItemInSlot( bow,		Player::EItemSlot_Slot1 );
	}
	
	return entity;
}

}
