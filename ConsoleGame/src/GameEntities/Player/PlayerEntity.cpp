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

#include "Weapons/Sword.h"
#include "Weapons/Bow.h"

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
	entity.AddComponent<RenderableComponent>( Player::kIdleMeshes[Player::EFacingDirection_Down] );
	entity.AddComponent<TriggererComponent>();
	
	entity.AddComponent<MessageReceiverComponent>()->Register<AttackMsg>
	(
		[entity] (const AttackMsg& inAttackMsg)
		{
			Player::OnAttacked(entity, inAttackMsg);
		}
	);
	
	auto playerComp = entity.AddComponent<PlayerComponent>(Player::EFacingDirection_Down);
	playerComp->AddWeapon( new Sword() );
	playerComp->AddWeapon( new Bow() );
	playerComp->SetSelectedWeapon( EWeapon_Bow );
	
	return entity;
}

}
