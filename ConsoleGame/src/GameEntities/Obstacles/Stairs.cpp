#include "Stairs.h"

#include <Core/Debug/Assert.h>

#include <EntityComponent/Components/CollisionComponent.h>
#include <EntityComponent/Components/PlayerComponent.h>
#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/RenderableComponent.h>
#include <EntityComponent/Components/TriggerBoxComponent.h>

#include <Containers/ContainerMacros.h>
#include <Messages/Messages.h>

namespace Stairs
{
	
static const char			kRenderMeshData[] = "|=====|";
static const AsciiMesh		kRenderMesh( kRenderMeshData, 7, 1 );
static const bool			kCollisionMeshData[] = 
{ 
	true,	true,	true,	true,	true,	true,	true,
	true,	false,	false,	false,	false,	false,	true 
};
static const CollisionMesh	kCollisionMesh(kCollisionMeshData, 7, 2, IVec2(0, -1));

Entity Create(World& inWorld, MessageBroadcaster& inMsgBroadcaster, const IVec2& inPosition)
{
	auto entity = inWorld.CreateEntity();
	
	entity.AddComponent<CollisionComponent>(kCollisionMesh);
	entity.AddComponent<PositionComponent>(inPosition);
	entity.AddComponent<RenderableComponent>(kRenderMesh);
	entity.AddComponent<TriggerBoxComponent>( IRect(1, 0, 5, 1) )->RegisterOnEnterCallback( [&] (const Entity&, const Entity& inTriggerer)
	{
		if (!inTriggerer.HasComponent<PlayerComponent>())
		{
			return;
		}

		inMsgBroadcaster.Broadcast( PlayerWentDownStairs() );
	} );

	return entity;
}

}
