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
#include <GameEntities/Monsters/MonsterEntityFactory.h>
#include <GameEntities/Obstacles/LockedDoor.h>
#include <GameEntities/Obstacles/DoorConstants.h>

#include "RoomEntityBuilder.h"
#include "ScreenConstants.h"

namespace DungeonFactory
{

enum
{
	ROOM_COL_COUNT = 3,
	ROOM_ROW_COUNT = 3,
};

struct RoomLink
{
	IVec2 mRoomIndex0;
	IVec2 mRoomIndex1;
};

static std::vector<RoomLink> sGenerateRoomLinks()
{
	std::vector<RoomLink> links;

	for (int col = 0; col < ROOM_COL_COUNT; ++col)
	{
		for (int row = 0; row < ROOM_ROW_COUNT-1; ++row)
		{
			RoomLink link;
			link.mRoomIndex0 = IVec2(col, row);
			link.mRoomIndex1 = IVec2(col, row+1);
			links.push_back(link);
		}
	}

	for (int col = 0; col < ROOM_COL_COUNT-1; ++col)
	{
		for (int row = 0; row < ROOM_ROW_COUNT; ++row)
		{
			RoomLink link;
			link.mRoomIndex0 = IVec2(col, row);
			link.mRoomIndex1 = IVec2(col+1, row);
			links.push_back(link);
		}
	}

	return links;
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
	SpawnRandomEntities(world, roomPos, rand()%4, [&] (const IVec2& inPos) { MonsterEntityFactory::Create(world, inMessageBroadcaster, inPos); } );
	SpawnRandomEntities(world, roomPos, rand()%5, [&] (const IVec2& inPos) { CoinEntity::Create(world, inMessageBroadcaster, inPos); } );
}

DungeonMap Generate(World& inWorld, MessageBroadcaster& inMessageBroadcaster)
{
	DungeonMap			dungeon(ROOM_COL_COUNT, ROOM_ROW_COUNT);
	RoomEntityBuilder	roomBuilder(inWorld);

	for (int col = 0; col < ROOM_COL_COUNT; ++col)
	{
		for (int row = 0; row < ROOM_ROW_COUNT; ++row)
		{
			IVec2 pos( col * ERoomDimensions_Width, row * ERoomDimensions_Height );

			int doorMask = EDoorMask_None;
			doorMask |= (col != 0)					? EDoorMask_Left	: EDoorMask_None;
			doorMask |= (row != 0)					? EDoorMask_Top		: EDoorMask_None;
			doorMask |= (col != (ROOM_COL_COUNT-1)) ? EDoorMask_Right	: EDoorMask_None;
			doorMask |= (row != (ROOM_ROW_COUNT-1)) ? EDoorMask_Bottom	: EDoorMask_None;

			auto room = roomBuilder.SetDoorMask((EDoorMask) doorMask).SetPosition(pos).Create();
			dungeon.Set(col, row, room);
			
			// Test locked doors
			auto createLockedDoorIfFlagSet = [&] (EDoorMask inMask, int inX, int inY, EOrientation inOrientation)
			{
				if ( (doorMask & inMask) > 0 )
				{
					LockedDoor::Create(inWorld, pos + IVec2(inX, inY), inOrientation);
				}
			};
	
			createLockedDoorIfFlagSet(EDoorMask_Top,	ERoomDimensions_DoorHorizOffset,	1,									EOrientation_FaceUp);
			createLockedDoorIfFlagSet(EDoorMask_Bottom, ERoomDimensions_DoorHorizOffset,	ERoomDimensions_Height-1,			EOrientation_FaceDown);
			createLockedDoorIfFlagSet(EDoorMask_Left,	1,									ERoomDimensions_DoorVertiOffset,	EOrientation_FaceLeft);
			createLockedDoorIfFlagSet(EDoorMask_Right,	ERoomDimensions_Width-2,			ERoomDimensions_DoorVertiOffset,	EOrientation_FaceRight);
		}
	}


	// Fill rooms after creating all rooms, to fix draw order problems.
	// A better way would be to either have an initial clearing screen pass then skip rendering any whitespace,
	// or render white space only if no other character has been written to a fragment yet,
	// or sort the renderables based on some sort of mark up on each renderable.
	dungeon.ForEach( [&] (size_t, size_t, const Entity& inRoom)
	{
		FillRoom(inRoom, inMessageBroadcaster);
	} );

	return dungeon;
}

}
