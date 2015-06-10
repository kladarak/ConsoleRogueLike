#include "PlayerEntity.h"

#include <EntityComponentSystem/World/World.h>

#include <EntityComponent/Components/CollisionComponent.h>
#include <EntityComponent/Components/InputHandlerComponent.h>
#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/RenderableComponent.h>
#include <EntityComponent/Components/TriggerBoxComponent.h>
#include <EntityComponent/Components/PlayerComponent.h>
#include <EntityComponent/Components/ProgramComponent.h>

#include "PlayerUpdateState.h"
#include "PlayerInputHandler.h"
#include "PlayerUpdater.h"
#include "PlayerMeshes.h"

namespace PlayerEntity
{

void Create(World& inWorld)
{
	Entity entity = inWorld.CreateEntity();

	entity.AddComponent<PositionComponent>( IVec2(10, 10) );
	entity.AddComponent<TriggererComponent>();
	entity.AddComponent<PlayerComponent>(Player::EFacingDirection_Down);
	entity.AddComponent<PlayerUpdateState>();
	entity.AddComponent<RenderableComponent>( Player::kIdleMeshes[Player::EFacingDirection_Down] );

	auto inputHandlerComp	= entity.AddComponent<InputHandlerComponent>();
	auto collisionComp		= entity.AddComponent<CollisionComponent>();
	auto programComp		= entity.AddComponent<ProgramComponent>();

	collisionComp->SetCollidableAt( IVec2(0, 0) );
	inputHandlerComp->RegisterHandler( &Player::HandleInput );
	programComp->RegisterProgram( &Player::UpdatePlayer );
}

}
