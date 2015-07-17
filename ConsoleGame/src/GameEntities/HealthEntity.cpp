#include "HealthEntity.h"

#include <Core/Messaging/MessageBroadcaster.h>

#include <EntityComponentSystem/World/World.h>
#include <EntityComponentSystem/World/EntityBuilder.h>

#include <EntityComponent/Components/HealthComponent.h>
#include <EntityComponent/Components/PlayerComponent.h>
#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/RenderableComponent.h>
#include <EntityComponent/Components/TriggerBoxComponent.h>

namespace HealthEntity
{

static const Fragment kHealth(3, ETextRed);
static const AsciiMesh kHealthMesh(&kHealth, 1, 1);


void OnEntityEntered(Entity inHealthEntity, const Entity& inPlayer)
{
	if (!inPlayer.HasComponent<PlayerComponent>())
	{
		return;
	}

	inPlayer.GetComponent<HealthComponent>()->IncHealth();
	inHealthEntity.Kill();
}

void Create(World& inWorld, const IVec2& inPosition)
{
	auto entity = EntityBuilder(inWorld)
					.AddComponent<PositionComponent>(inPosition)
					.AddComponent<RenderableComponent>(kHealthMesh)
					.AddComponent<TriggerBoxComponent>( IRect(0, 0, 1, 1) )
					.Create();
	
	entity.GetComponent<TriggerBoxComponent>()->RegisterOnEnterCallback( &OnEntityEntered );
}

}
