#include "CoinEntity.h"

#include <EntityComponentSystem/World/World.h>

#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/TriggerBoxComponent.h>
#include <EntityComponent/Components/RenderableComponent.h>
#include <EntityComponent/Components/PlayerComponent.h>

namespace CoinEntity
{

static const char kCoinSprite = '$';

static void OnEntityEntered(const Entity& inCoinEntity, const Entity& inTriggerer)
{
	if (!inTriggerer.HasComponent<PlayerComponent>())
	{
		return;
	}

	// This gets around the constness....
	inCoinEntity.GetWorld()->DestroyEntity(inCoinEntity.GetID());
}

void Create(World& inWorld, const IVec2& inPos)
{
	Entity entity = inWorld.CreateEntity();

	entity.AddComponent<PositionComponent>(inPos);

	auto triggerBoxComp	= entity.AddComponent<TriggerBoxComponent>( IRect(0, 0, 1, 1) );
	triggerBoxComp->RegisterOnEnterCallback( &OnEntityEntered );

	AsciiMesh mesh;
	mesh.SetCharAtPosition(IVec2(0, 0), kCoinSprite);
	auto renderableComp	= entity.AddComponent<RenderableComponent>();
	renderableComp->SetMesh(mesh);
}

}
