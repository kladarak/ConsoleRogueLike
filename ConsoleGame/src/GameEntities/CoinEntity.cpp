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

static void OnEntityEntered(MessageBroadcaster& inMessageBroadcaster, Entity inCoinEntity, Entity inTriggerer)
{
	if (!inTriggerer.HasComponent<PlayerComponent>())
	{
		return;
	}

	inMessageBroadcaster.Broadcast( CoinCollectedMessage() );

	inCoinEntity.Kill();
}

void Create(World& inWorld, MessageBroadcaster& inMessageBroadcaster, const IVec2& inPos)
{
	Entity entity = inWorld.CreateEntity();

	entity.AddComponent<PositionComponent>(inPos);

	entity.AddComponent<TriggerBoxComponent>( IRect(0, 0, 1, 1) )->RegisterOnEnterCallback
	( 
		[&] (Entity inCoinEntity, Entity inTriggerer) 
		{
			OnEntityEntered(inMessageBroadcaster, inCoinEntity, inTriggerer);
		}
	);

	entity.AddComponent<RenderableComponent>( kCoinSprite );
}

}
