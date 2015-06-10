#include "CoinEntity.h"

#include <Core/Messaging/MessageBroadcaster.h>

#include <EntityComponentSystem/World/World.h>

#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/TriggerBoxComponent.h>
#include <EntityComponent/Components/RenderableComponent.h>
#include <EntityComponent/Components/PlayerComponent.h>

#include <Messages/Messages.h>

namespace CoinEntity
{

static const char kCoinSprite = '$';

static void OnEntityEntered(MessageBroadcaster& inMessageBroadcaster, const Entity& inCoinEntity, const Entity& inTriggerer)
{
	if (!inTriggerer.HasComponent<PlayerComponent>())
	{
		return;
	}

	inMessageBroadcaster.Broadcast( CoinCollectedMessage() );

	// This gets around the constness....
	inCoinEntity.GetWorld()->DestroyEntity(inCoinEntity.GetID());
}

void Create(World& inWorld, MessageBroadcaster& inMessageBroadcaster, const IVec2& inPos)
{
	Entity entity = inWorld.CreateEntity();

	entity.AddComponent<PositionComponent>(inPos);

	auto triggerBoxComp	= entity.AddComponent<TriggerBoxComponent>( IRect(0, 0, 1, 1) );
	triggerBoxComp->RegisterOnEnterCallback( [&] (const Entity& inCoinEntity, const Entity& inTriggerer) 
	{
		OnEntityEntered(inMessageBroadcaster, inCoinEntity, inTriggerer);
	});

	AsciiMesh mesh;
	mesh.SetCharAtPosition(0, 0, kCoinSprite);
	entity.AddComponent<RenderableComponent>(mesh);
}

}
