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

void CameraSystem::Init(World& inWorld, DungeonMap& inDungeonMap)
{
	mCameraEntity = inWorld.CreateEntity();
	mCameraEntity.AddComponent<CameraComponent>();
	mCameraEntity.AddComponent<PositionComponent>( IVec2(0, 0) );

	inDungeonMap.GetRoomEntities().ForEach( [&] (size_t inCol, size_t inRow, const Entity& inRoom)
	{
		if (inRoom.IsValid())
		{
			auto triggerBox = inRoom.GetComponent<TriggerBoxComponent>();
			triggerBox->RegisterOnEnterCallback( [=, &inDungeonMap] (const Entity& inRoom, const Entity& inTriggerer) 
			{
				if (inTriggerer.HasComponent<PlayerComponent>())
				{
					MoveCameraToRoom(inRoom);
					inDungeonMap.SetRoomVisited(inCol, inRow);
				}
			} );
		}
	} );
}

void CameraSystem::MoveCameraToRoom(const Entity& inRoom)
{
	auto posComp = inRoom.GetComponent<PositionComponent>();
	assert(nullptr != posComp);

	auto position = posComp->GetPosition();
	mCameraEntity.GetComponent<PositionComponent>()->SetPosition(position);
}

IVec2 CameraSystem::GetCameraPosition() const
{
	return mCameraEntity.GetComponent<PositionComponent>()->GetPosition();
}
