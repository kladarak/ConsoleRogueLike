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

#include <Inventory/Items/DoorKey.h>

#include "RoomEntity.h"
#include "ScreenConstants.h"
#include "DungeonLayoutGenerator.h"
#include "DungeonDoorPlacer.h"

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

static void SpawnRandomEntities(World& inWorld, const IVec2& inRoomPos, int inCount, const std::function<void (const IVec2& inPos)>& inEntitySpawner)
{
	for (int i = 0; i < inCount; ++i)
	{
		int x = rand() % ERoomDimensions_Width;
		int y = rand() % ERoomDimensions_Height;
		IVec2 entityPos(x, y);
		entityPos += inRoomPos;

		if ( !CollisionSystem::CollidesWithAnyEntity(inWorld, entityPos) )
		{
			inEntitySpawner(entityPos);
		}
	}
}

static void FillRoom(Entity inRoom, MessageBroadcaster& inMessageBroadcaster)
{
	World& world = *inRoom.GetWorld();
	auto roomPos = inRoom.GetComponent<PositionComponent>()->GetPosition();
	
	SpawnRandomEntities(world, roomPos, rand()%4, [&] (const IVec2& inPos) { SpinnerEntity::Create(world, inPos); } );
	SpawnRandomEntities(world, roomPos, rand()%4, [&] (const IVec2& inPos) { WormMonster::Create(world, inMessageBroadcaster, inPos); } );
	SpawnRandomEntities(world, roomPos, rand()%5, [&] (const IVec2& inPos) { CoinEntity::Create(world, inMessageBroadcaster, inPos); } );
	SpawnRandomEntities(world, roomPos, rand()%3, [&] (const IVec2& inPos) { WallSparkMonster::Create(world, inMessageBroadcaster, inPos); } );
}

DungeonMap Generate(World& inWorld, MessageBroadcaster& inMessageBroadcaster, GameData* inGameData)
{
	const int kRoomCount = 5;

	Dynamic2DVector<RoomData>	roomData		= DungeonLayoutGenerator(kRoomCount).Generate();
	Dynamic2DVector<Entity>		roomEntities	= sCreateRoomEntities(inWorld, roomData);

	DungeonMap dungeon(roomData, roomEntities);

	sAddDoors(dungeon, inWorld, inMessageBroadcaster);

	// Place key somewhere.
	{
		auto doorKey = ItemEntity::Create<DoorKey>(inWorld, inGameData);
		sPlaceItemInDungeon(doorKey, dungeon);
	}

	// Fill rooms after creating all rooms, to fix draw order problems.
	// A better way would be to either have an initial clearing screen pass then skip rendering any whitespace,
	// or render white space only if no other character has been written to a fragment yet,
	// or sort the renderables based on some sort of mark up on each renderable.
	dungeon.GetRoomEntities().ForEach( [&] (size_t, size_t, const Entity& inRoom)
	{
		if (inRoom.IsValid())
		{
			FillRoom(inRoom, inMessageBroadcaster);
		}
	} );

	return dungeon;
}

}
