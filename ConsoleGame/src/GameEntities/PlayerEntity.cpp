#include "PlayerEntity.h"

#include <EntityComponentSystem/World/World.h>

#include <EntityComponent/Components/CollisionComponent.h>
#include <EntityComponent/Components/InputHandlerComponent.h>
#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/RenderableComponent.h>
#include <EntityComponent/Components/TriggerBoxComponent.h>
#include <EntityComponent/Components/PlayerComponent.h>

#include <EntityComponent/Systems/CollisionSystem.h>

#include <Input/InputBuffer.h>

namespace PlayerEntity
{

static const AsciiMesh kPlayerIdleMeshes[] =
{
	AsciiMesh( { { IVec2(0, 0), 'O' } } ),
	AsciiMesh( { { IVec2(0, 0), 'O' } } ),
	AsciiMesh( { { IVec2(0, 0), 'O' } } ),
	AsciiMesh( { { IVec2(0, 0), 'O' } } ),
};

static const AsciiMesh kPlayerSwordMeshes[] =
{
	AsciiMesh( { { IVec2(0, 0), 'O' }, { IVec2( 0, -1), '\\' } } ),
	AsciiMesh( { { IVec2(0, 0), 'O' }, { IVec2( 0,  1), '\\' } } ),
	AsciiMesh( { { IVec2(0, 0), 'O' }, { IVec2(-1,  0), '/' } } ),
	AsciiMesh( { { IVec2(0, 0), 'O' }, { IVec2( 1,  0), '/' } } ),
};

static const AsciiMesh kPlayerShieldMeshes[] =
{
	AsciiMesh( { { IVec2(0, 0), 'O' }, { IVec2( 0, -1), '-' } } ),
	AsciiMesh( { { IVec2(0, 0), 'O' }, { IVec2( 0,  1), '-' } } ),
	AsciiMesh( { { IVec2(0, 0), 'O' }, { IVec2(-1,  0), '|' } } ),
	AsciiMesh( { { IVec2(0, 0), 'O' }, { IVec2( 1,  0), '|' } } ),
};

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
		facingDirection = PlayerComponent::EFacingDirection_Left;
	}
	else if ( inBuffer.IsPressed('d') )
	{
		currentPos.mX += 1;
		facingDirection = PlayerComponent::EFacingDirection_Right;
	}
	
	if ( inBuffer.IsPressed('w') )
	{
		currentPos.mY -= 1;
		facingDirection = PlayerComponent::EFacingDirection_Up;
	}
	else if ( inBuffer.IsPressed('s') )
	{
		currentPos.mY += 1;
		facingDirection = PlayerComponent::EFacingDirection_Down;
	}
	
	if (inBuffer.IsPressed(' '))
	{
		state = PlayerComponent::EState_Attacking;
	}
	else if (inBuffer.IsPressed('e'))
	{
		state = PlayerComponent::EState_Defending;
	}
	else
	{
		state = PlayerComponent::EState_Idle;
	}
	
	if ((state != playerComp->GetState()) || facingDirection != playerComp->GetFacingDirection())
	{
		playerComp->SetFacingDirection(facingDirection);
		playerComp->SetState(state);

		auto meshes = [state] () -> const AsciiMesh*
		{
			switch (state)
			{
				case PlayerComponent::EState_Idle:		return kPlayerIdleMeshes;
				case PlayerComponent::EState_Attacking: return kPlayerSwordMeshes;
				case PlayerComponent::EState_Defending: return kPlayerShieldMeshes;
			}
		} ();

		auto renderableComp	= inThis.GetComponent<RenderableComponent>();
		renderableComp->SetMesh( meshes[facingDirection] );
	}
	
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
	entity.AddComponent<PlayerComponent>(PlayerComponent::EFacingDirection_Down);

	auto inputHandlerComp	= entity.AddComponent<InputHandlerComponent>();
	auto collisionComp		= entity.AddComponent<CollisionComponent>();
	auto renderableComp		= entity.AddComponent<RenderableComponent>();

	inputHandlerComp->RegisterHandler( &HandleInput );
	collisionComp->SetCollidableAt( IVec2(0, 0) );
	renderableComp->SetMesh( kPlayerIdleMeshes[PlayerComponent::EFacingDirection_Down] );
}

}
