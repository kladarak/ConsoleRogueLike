#include "PlayerEntity.h"

#include <EntityComponentSystem/World/World.h>

#include <EntityComponent/Components/CollisionComponent.h>
#include <EntityComponent/Components/InputHandlerComponent.h>
#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/RenderableComponent.h>
#include <EntityComponent/Components/TriggerBoxComponent.h>
#include <EntityComponent/Components/PlayerComponent.h>
#include <EntityComponent/Components/ProgramComponent.h>

#include <EntityComponent/Systems/CollisionSystem.h>

#include <Input/InputBuffer.h>

#include "PlayerUpdater.h"
#include "PlayerUpdateState.h"
#include "PlayerMeshes.h"

namespace PlayerEntity
{

static void HandleInput(const Entity& inThis, const InputBuffer& inBuffer)
{
	auto	positionComp	= inThis.GetComponent<PositionComponent>();
	auto	playerComp		= inThis.GetComponent<PlayerComponent>();
	
	IVec2	currentPos		= positionComp->GetPosition();
	auto	facingDirection	= playerComp->GetFacingDirection();
	auto	state			= playerComp->GetState();

	if ( inBuffer.IsPressed('a') )
	{
		currentPos.mX -= 1;
		facingDirection = Player::EFacingDirection_Left;
	}
	else if ( inBuffer.IsPressed('d') )
	{
		currentPos.mX += 1;
		facingDirection = Player::EFacingDirection_Right;
	}
	
	if ( inBuffer.IsPressed('w') )
	{
		currentPos.mY -= 1;
		facingDirection = Player::EFacingDirection_Up;
	}
	else if ( inBuffer.IsPressed('s') )
	{
		currentPos.mY += 1;
		facingDirection = Player::EFacingDirection_Down;
	}
	
	if (inBuffer.IsPressed(' '))
	{
		state = Player::EState_Attacking;
	}
	else if (inBuffer.IsPressed('e'))
	{
		state = Player::EState_Defending;
	}
	
	playerComp->SetFacingDirection(facingDirection);
	playerComp->SetState(state);
	
	if (currentPos != positionComp->GetPosition())
	{
		if ( !CollisionSystem::CollidesWithAnyEntity(*inThis.GetWorld(), inThis, currentPos) )
		{
			positionComp->SetPosition(currentPos);
		}
	}
}

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

	inputHandlerComp->RegisterHandler( &HandleInput );
	collisionComp->SetCollidableAt( IVec2(0, 0) );
	programComp->RegisterProgram( &Player::UpdatePlayer );
}

}
