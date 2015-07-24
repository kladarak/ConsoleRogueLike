#include "DungeonFactory.h"

#include <string>
#include <algorithm>

#include <EntityComponentSystem/World/World.h>

#include <EntityComponent/Components/CollisionComponent.h>
#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/RenderableComponent.h>
#include <EntityComponent/Components/TriggerBoxComponent.h>

#include <EntityComponent/Systems/CollisionSystem.h>

#include <GameEntities/CoinEntity.h>
#include <GameEntities/ItemEntity.h>
#include <GameEntities/Other/HeartContainer.h>

#include <Containers/ContainerMacros.h>

#include <Inventory/Items/DoorKey.h>

#include "RoomEntity.h"
#include "ScreenConstants.h"
#include "DungeonLayoutGenerator.h"
#include "DungeonDoorPlacer.h"
#include "DungeonLockedDoorsAndKeysPlacer.h"
#include "DungeonGeneratorUtils.h"
#include "LevelData.h"

using namespace DungeonGeneratorUtils;

namespace DungeonFactory
{

static Dynamic2DVector<Entity> sCreateRoomEntities(World& inWorld, const Dynamic2DVector<RoomData>& inLayout)
{
	Dynamic2DVector<Entity> entities;

	inLayout.ForEach( [&] (size_t inCol, size_t inRow, const RoomData& inRoomData)
	{
		if (inRoomData.mIsValid)
		{
			IVec2 pos( inCol * ERoomDimensions_Width, inRow * ERoomDimensions_Height );
			auto room = RoomEntity::Create(inWorld, pos);
			entities.Set(inCol, inRow, room);
		}
	} );

	return entities;
}

static void sAddDoors(const DungeonMap& inDungeon, World& inWorld, MessageBroadcaster& inMessageBroadcaster)
{
	DungeonDoorPlacer doorPlacer(inWorld, inDungeon);

	auto doorLinks = DungeonDoorPlacer::sGenerateRoomLinks(inDungeon);
	for (auto& link : doorLinks)
	{
		doorPlacer.AddOpenDoor(link);
	}

	// Construct stairs, by finding a room without a door on its top side.
	IVec2 roomIndex(-1, -1);

	do
	{
		roomIndex = sGetRandomValidRoomIndex(inDungeon);
	}
	while ( inDungeon.GetRoomData().Get(roomIndex).mDoors[EDoorSide_Top] );
		
	doorPlacer.AddDungeonExit(roomIndex, inMessageBroadcaster);
}

static void sPlaceItemInDungeon(const DungeonMap& inDungeon, Entity inItem)
{
	IVec2 position = sGetRandomValidRoomPosition(inDungeon);
	position += sGetRandomOffsetInRoom( inItem.GetComponent<TriggerBoxComponent>()->GetBounds() );
	inItem.GetComponent<PositionComponent>()->SetPosition(position);
}

static void sCreateMoneyInDungeon(const DungeonMap& inDungeonMap, World& inWorld, MessageBroadcaster& inMessageBroadcaster, int inCount)
{
	for (int i = 0; i < inCount; ++i)
	{
		IVec2 position = sGetRandomValidRoomPosition(inDungeonMap) + sGetRandomOffsetInRoom();
		CoinEntity::Create(inWorld, inMessageBroadcaster, position);
	}
}

static void sCreateHeartContainerInDungeon(const DungeonMap& inDungeon, World& inWorld, GameData* inGameData)
{
	auto heartContainer = HeartContainer::Create(inWorld, inGameData);
	auto& mesh = heartContainer.GetComponent<RenderableComponent>()->GetMesh();
	IVec2 position = sGetRandomValidRoomPosition(inDungeon) + sGetRandomOffsetInRoom( mesh.GetLocalBounds() );
	heartContainer.GetComponent<PositionComponent>()->SetPosition(position);
}

static void sCreateMonstersInDungeon(const DungeonMap& inDungeonMap, World& inWorld, MessageBroadcaster& inMessageBroadcaster, const std::vector<MonsterSpawnInfo>& inMonsterInfo )
{
	for (auto& monsterInfo : inMonsterInfo)
	{
		for (int i = 0; i < monsterInfo.mMonsterCount; ++i)
		{
			IVec2 entityPos(0, 0);

			do
			{
				entityPos = sGetRandomValidRoomPosition(inDungeonMap) + sGetRandomOffsetInRoom();
			}
			while ( CollisionSystem::CollidesWithAnyEntity(inWorld, entityPos) );

			monsterInfo.mFactoryFunc(inWorld, inMessageBroadcaster, entityPos);
		}
	}
}

DungeonMap Generate(World& inWorld, MessageBroadcaster& inMessageBroadcaster, GameData* inGameData, const LevelData& inLevelData)
{
	Dynamic2DVector<RoomData>	roomData		= DungeonLayoutGenerator( inLevelData.mRoomCount ).Generate();
	Dynamic2DVector<Entity>		roomEntities	= sCreateRoomEntities(inWorld, roomData);

	DungeonMap dungeon(roomData, roomEntities);

	IVec2 playerStartingRoomIndex = sGetRandomValidRoomIndex(dungeon);
	dungeon.SetPlayerStartingRoomIndex( playerStartingRoomIndex );

	sAddDoors(dungeon, inWorld, inMessageBroadcaster);

	DungeonLockedDoorsAndKeysPlacer::sPlaceThem(dungeon, inWorld, inGameData, inLevelData.mLockedDoorsCount);

	sPlaceItemInDungeon(dungeon, ItemEntity::Create< ColouredDoorKey<ETextDarkYellow> >(inWorld, inGameData));

	if (inLevelData.mItemFactoryFunc)
	{
		sPlaceItemInDungeon(dungeon, inLevelData.mItemFactoryFunc(inWorld, inGameData));
	}
	
	if (inLevelData.mAddHeartContainer)
	{
		sCreateHeartContainerInDungeon(dungeon, inWorld, inGameData);
	}

	sCreateMoneyInDungeon(dungeon, inWorld, inMessageBroadcaster, inLevelData.mMoneyCount);

	sCreateMonstersInDungeon(dungeon, inWorld, inMessageBroadcaster, inLevelData.mMonsterSpawnInfo);

	return dungeon;
}

}
