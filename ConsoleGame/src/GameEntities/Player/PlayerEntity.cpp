#include "PlayerEntity.h"

#include <Core/Messaging/MessageBroadcaster.h>

#include <EntityComponentSystem/World/World.h>

#include <EntityComponent/Components/AnimationComponent.h>
#include <EntityComponent/Components/CollisionComponent.h>
#include <EntityComponent/Components/HealthComponent.h>
#include <EntityComponent/Components/InputHandlerComponent.h>
#include <EntityComponent/Components/MessageReceiverComponent.h>
#include <EntityComponent/Components/PlayerComponent.h>
#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/ProgramComponent.h>
#include <EntityComponent/Components/TriggerBoxComponent.h>
#include <EntityComponent/Components/RenderableComponent.h>

#include <Messages/Messages.h>

#include "Items/Sword.h"
#include "Items/Bow.h"
#include "Items/Shield.h"

#include "PlayerMeshes.h"

namespace PlayerEntity
{

Entity Create(World& inWorld)
{
	Entity entity = inWorld.CreateEntity();
	
	entity.AddComponent<AnimationComponent>( Player::kIdleAnimations, gElemCount(Player::kIdleAnimations) );
	entity.AddComponent<CollisionComponent>()->SetCollidableAt(0, 0);
	entity.AddComponent<HealthComponent>(3);
	entity.AddComponent<PlayerComponent>(Player::EFacingDirection_Down);
	entity.AddComponent<PositionComponent>(10, 10);
	entity.AddComponent<RenderableComponent>( Player::kIdleRenderMesh );
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
		[entity] (const AttackMsg& inAttackMsg)
		{
			entity.GetComponent<PlayerComponent>()->OnAttacked(entity, inAttackMsg);
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

		playerComp->SetItemInSlot( sword,	Player::EItemSlot_Slot0 );
		playerComp->SetItemInSlot( bow,		Player::EItemSlot_Slot1 );
	}
	
	return entity;
}

}
