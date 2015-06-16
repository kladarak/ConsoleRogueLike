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

#include "PlayerUpdateState.h"
#include "PlayerInputHandler.h"
#include "PlayerUpdater.h"
#include "PlayerMeshes.h"

namespace PlayerEntity
{

Entity Create(World& inWorld)
{
	Entity entity = inWorld.CreateEntity();
	
	entity.AddComponent<AnimationComponent>( Player::kAnimations, gElemCount(Player::kAnimations) );
	entity.AddComponent<CollisionComponent>()->SetCollidableAt( IVec2(0, 0) );
	entity.AddComponent<HealthComponent>(3);
	entity.AddComponent<InputHandlerComponent>()->RegisterHandler( &Player::HandleInput );
	entity.AddComponent<PlayerUpdateState>();
	entity.AddComponent<PositionComponent>( IVec2(10, 10) );
	entity.AddComponent<ProgramComponent>()->RegisterProgram( &Player::UpdatePlayer );
	entity.AddComponent<RenderableComponent>( Player::kIdleMesh );
	entity.AddComponent<TriggererComponent>();
	
	entity.AddComponent<MessageReceiverComponent>()->Register<AttackMsg>
	(
		[entity] (const AttackMsg& inAttackMsg)
		{
			Player::OnAttacked(entity, inAttackMsg);
		}
	);
	
	auto playerComp = entity.AddComponent<PlayerComponent>(Player::EFacingDirection_Down);
	auto& inventory = playerComp->GetInventory();

	auto sword	= new Sword();
	auto bow	= new Bow();
	inventory.AddItem( sword );
	inventory.AddItem( bow );
	inventory.AddItem( new Shield() );

	playerComp->SetItemInSlot1( sword );
	playerComp->SetItemInSlot2( bow );
	
	return entity;
}

}
