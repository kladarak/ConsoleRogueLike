#include "DungeonFactory.h"

#include <string>

#include <EntityComponentSystem/World/World.h>

#include <EntityComponent/Components/CollisionComponent.h>
#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/RenderableComponent.h>
#include <EntityComponent/Components/TriggerBoxComponent.h>

#include <EntityComponent/Systems/CollisionSystem.h>

#include <GameEntities/CoinEntity.h>
#include <GameEntities/SpinnerEntity.h>
#include <GameEntities/ItemEntity.h>
#include <GameEntities/Monsters/WormMonster.h>
#include <GameEntities/Monsters/WallSparkMonster.h>
#include <GameEntities/Monsters/ArcherMonster.h>

#include <Inventory/Items/DoorKey.h>

#include "RoomEntity.h"
#include "ScreenConstants.h"
#include "DungeonLayoutGenerator.h"
#include "DungeonDoorPlacer.h"
#include "LevelData.h"

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

static IVec2 sGetRandomValidRoomIndex(const DungeonMap& inDungeon)
{
	int col, row;

	do
	{
		col = rand() % inDungeon.GetColCount();
		row = rand() % inDungeon.GetRowCount();
	}
	while ( !inDungeon.GetRoomData().Get(col, row).mIsValid );

	return IVec2(col, row);
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

static void sPlaceItemInDungeon(Entity inItem, const DungeonMap& inDungeon)
{
	IVec2 position(0, 0);

	// Select room and set position to room position
	{
		IVec2	roomIndex	= sGetRandomValidRoomIndex(inDungeon);
		auto	room		= inDungeon.GetRoomEntities().Get(roomIndex);
		position			= room.GetComponent<PositionComponent>()->GetPosition();
	}

	// Generate random offset considering wall/door margins and item size.
	{
		auto const& keyBounds = inItem.GetComponent<TriggerBoxComponent>()->GetBounds();
		
		static const int kRoomMargin = 2;
		int validAreaWidth = ERoomDimensions_Width - (kRoomMargin*2 + keyBounds.mWidth);
		int validAreaHeight = ERoomDimensions_Height - (kRoomMargin*2 + keyBounds.mHeight);
		int x = rand() % validAreaWidth;
		int y = rand() % validAreaHeight;
		x += kRoomMargin + keyBounds.mX;
		y += kRoomMargin + keyBounds.mY;
		position.mX += x;
		position.mY += y;
	}

	inItem.GetComponent<PositionComponent>()->SetPosition(position);
}

static void sFillDungeonWithMonsters(World& inWorld, MessageBroadcaster& inMessageBroadcaster, const DungeonMap& inDungeonMap, const std::vector<MonsterSpawnInfo>& inMonsterInfo )
{
	for (auto& monsterInfo : inMonsterInfo)
	{
		for (int i = 0; i < monsterInfo.mMonsterCount; ++i)
		{
			IVec2 entityPos(0, 0);

			do
			{
				IVec2 roomIndex		= sGetRandomValidRoomIndex(inDungeonMap);
				IVec2 roomPosition	= inDungeonMap.GetRoomEntities().Get(roomIndex).GetComponent<PositionComponent>()->GetPosition();
			
				int x = rand() % ERoomDimensions_Width;
				int y = rand() % ERoomDimensions_Height;
				entityPos = IVec2(x, y) + roomPosition;
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

	sAddDoors(dungeon, inWorld, inMessageBroadcaster);

	sPlaceItemInDungeon( ItemEntity::Create<DoorKey>(inWorld, inGameData), dungeon );

	sFillDungeonWithMonsters(inWorld, inMessageBroadcaster, dungeon, inLevelData.mMonsterSpawnInfo);

	return dungeon;
}

}
