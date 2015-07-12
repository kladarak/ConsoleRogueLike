#include "CameraSystem.h"

#include <EntityComponentSystem/World/World.h>
#include <Dungeon/DungeonMap.h>

#include <EntityComponent/Components/CameraComponent.h>
#include <EntityComponent/Components/PlayerComponent.h>
#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/TriggerBoxComponent.h>

CameraSystem::CameraSystem()
	: mCameraEntity(0, nullptr)
{
}

void CameraSystem::Init(World& inWorld, const DungeonMap& inDungeonMap)
{
	mCameraEntity = inWorld.CreateEntity();
	mCameraEntity.AddComponent<CameraComponent>();
	mCameraEntity.AddComponent<PositionComponent>( IVec2(0, 0) );

	inDungeonMap.GetRoomEntities().ForEach( [&] (size_t, size_t, const Entity& inRoom)
	{
		if (inRoom.IsValid())
		{
			auto triggerBox = inRoom.GetComponent<TriggerBoxComponent>();
			triggerBox->RegisterOnEnterCallback( [this] (const Entity& inRoom, const Entity& inTriggerer) { OnEntityEnteredRoom(inRoom, inTriggerer); } );
		}
	} );
}

void CameraSystem::OnEntityEnteredRoom(const Entity& inRoom, const Entity& inTriggerer)
{
	if (!inTriggerer.HasComponent<PlayerComponent>())
	{
		return;
	}

	auto posComp = inRoom.GetComponent<PositionComponent>();
	assert(nullptr != posComp);

	auto position = posComp->GetPosition();
	mCameraEntity.GetComponent<PositionComponent>()->SetPosition(position);
}

IVec2 CameraSystem::GetCameraPosition() const
{
	return mCameraEntity.GetComponent<PositionComponent>()->GetPosition();
}
