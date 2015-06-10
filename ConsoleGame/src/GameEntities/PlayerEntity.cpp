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

static void HandleInput(const Entity& inThis, const InputBuffer& inBuffer)
{
	auto positionComp	= inThis.GetComponent<PositionComponent>();
	IVec2 currentPos	= positionComp->GetPosition();

	if ( inBuffer.IsPressed('a') )
	{
		currentPos.mX -= 1;
	}
	else if ( inBuffer.IsPressed('d') )
	{
		currentPos.mX += 1;
	}
	
	if ( inBuffer.IsPressed('w') )
	{
		currentPos.mY -= 1;
	}
	else if ( inBuffer.IsPressed('s') )
	{
		currentPos.mY += 1;
	}

	if ( !CollisionSystem::CollidesWithAnyEntity(*inThis.GetWorld(), inThis, currentPos) )
	{
		positionComp->SetPosition(currentPos);
	}
}

void Create(World& inWorld)
{
	Entity entity = inWorld.CreateEntity();

	auto positionComp		= entity.AddComponent<PositionComponent>();
	auto inputHandlerComp	= entity.AddComponent<InputHandlerComponent>();
	auto collisionComp		= entity.AddComponent<CollisionComponent>();
	auto renderableComp		= entity.AddComponent<RenderableComponent>();
	entity.AddComponent<TriggererComponent>();
	entity.AddComponent<PlayerComponent>();

	positionComp->SetPosition( IVec2(10, 10) );
	inputHandlerComp->RegisterHandler( &HandleInput );
	collisionComp->SetCollidableAt( IVec2(0, 0) );
	
	AsciiMesh mesh;
	mesh.SetCharAtPosition(IVec2(0, 0), 2); //2 is a smiley face on my console...
	renderableComp->SetMesh(mesh);
}

}
