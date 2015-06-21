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

#include "ScreenConstants.h"

namespace DungeonFactory
{

enum
{
	ROOM_WIDTH	= ScreenConstants::EMapCols+1,
	ROOM_HEIGHT	= ScreenConstants::EMapRows,
	DOOR_HORIZ_OFFSET = (ROOM_WIDTH - EDoorSize_Width) / 2,
	DOOR_VERTI_OFFSET = ((ROOM_HEIGHT - EDoorSize_Height) / 2) + 1,

	ROOM_COL_COUNT = 3,
	ROOM_ROW_COUNT = 3,
};

static const std::string kWhiteSpace = " \n\r\t\0";
static const char kRoomWallHoriz[ROOM_WIDTH+1] = " _____________________________________ "; //+1 for terminating character...
static const char kRoomWallVerti[ROOM_HEIGHT] =
{
	' ',
	'|',
	'|',
	'|',
	'|',
	'|',
	'|',
	'|',
	'|',
	'|',
	'|',
	'|',
	'|',
	'|'
};

enum EDoorMask
{
	EDoorMask_None		= 0,
	EDoorMask_Top		= 1 << 0,
	EDoorMask_Bottom	= 1 << 1,
	EDoorMask_Left		= 1 << 2,
	EDoorMask_Right		= 1 << 3,
};

struct RoomLink
{
	IVec2 mRoomIndex0;
	IVec2 mRoomIndex1;
};

static AsciiMesh sGenerateRoom()
{
	AsciiMesh mesh(' ', ROOM_HEIGHT, ROOM_WIDTH);
	
	for (int i = 0; i < ROOM_WIDTH; ++i)	{ mesh.SetCharAtPosition(i,				0,				kRoomWallHoriz[i]); }
	for (int i = 0; i < ROOM_WIDTH; ++i)	{ mesh.SetCharAtPosition(i,				ROOM_HEIGHT-1, 	kRoomWallHoriz[i]); }
	for (int i = 0; i < ROOM_HEIGHT; ++i)	{ mesh.SetCharAtPosition(0,				i,				kRoomWallVerti[i]); }
	for (int i = 0; i < ROOM_HEIGHT; ++i)	{ mesh.SetCharAtPosition(ROOM_WIDTH-1,	i,				kRoomWallVerti[i]); }

	return mesh;
}

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

static void sSetupDoors(AsciiMesh& inMesh, EDoorMask inDoorMask)
{
	auto clearPositions = [&] (EDoorMask inMask, size_t inLoopCount, const std::function<IVec2 (size_t)>& inPosGetter)
	{
		if ( (inDoorMask & inMask) > 0 )
		{
			for (size_t i = 0; i < inLoopCount; ++i)
			{
				IVec2 pos = inPosGetter(i);
				inMesh.SetCharAtPosition(pos.mX, pos.mY, ' ');
			}
		}
	};

	clearPositions(EDoorMask_Top,		EDoorSize_Width,	[] (size_t i) { return IVec2(DOOR_HORIZ_OFFSET + i, 0); } );
	clearPositions(EDoorMask_Bottom,	EDoorSize_Width,	[] (size_t i) { return IVec2(DOOR_HORIZ_OFFSET + i, ROOM_HEIGHT-1); } );
	clearPositions(EDoorMask_Left,		EDoorSize_Height-2, [] (size_t i) { return IVec2(0, DOOR_VERTI_OFFSET+i+1); } );
	clearPositions(EDoorMask_Right,		EDoorSize_Height-2, [] (size_t i) { return IVec2(ROOM_WIDTH-1, DOOR_VERTI_OFFSET+i+1); } );
}

static Entity CreateRoom(World& inWorld, EDoorMask inDoorMask, const IVec2& inPosition)
{
	Entity entity = inWorld.CreateEntity();

	entity.AddComponent<PositionComponent>( inPosition );
	entity.AddComponent<TriggerBoxComponent>( IRect(0, 0, ROOM_WIDTH, ROOM_HEIGHT) );

	auto collisionComp	= entity.AddComponent<CollisionComponent>();
	auto renderableComp	= entity.AddComponent<RenderableComponent>();
	
	AsciiMesh renderMesh = sGenerateRoom();
	sSetupDoors(renderMesh, inDoorMask);
	renderableComp->SetMesh(renderMesh);

	CollisionMesh collisionMesh;

	renderMesh.ForEachFrag( [&] (int inX, int inY, char inChar)
	{
		if ( kWhiteSpace.find( inChar ) == std::string::npos )
		{
			collisionMesh.SetCollidableAt( inX, inY );
		}
	} );

	collisionComp->SetDefaultCollisionMesh(collisionMesh);

	auto createLockedDoorIfFlagSet = [&] (EDoorMask inMask, int inX, int inY, EOrientation inOrientation)
	{
		if ( (inDoorMask & inMask) > 0 )
		{
			LockedDoor::Create(inWorld, inPosition + IVec2(inX, inY), inOrientation);
		}
	};
	
	createLockedDoorIfFlagSet(EDoorMask_Top,	DOOR_HORIZ_OFFSET,	1,					EOrientation_FaceUp);
	createLockedDoorIfFlagSet(EDoorMask_Bottom, DOOR_HORIZ_OFFSET,	ROOM_HEIGHT-1,		EOrientation_FaceDown);
	createLockedDoorIfFlagSet(EDoorMask_Left,	1,					DOOR_VERTI_OFFSET,	EOrientation_FaceLeft);
	createLockedDoorIfFlagSet(EDoorMask_Right,	ROOM_WIDTH-2,		DOOR_VERTI_OFFSET,	EOrientation_FaceRight);

	return entity;
}

static void SpawnRandomEntities(World& inWorld, const IVec2& inRoomPos, int inCount, const std::function<void (const IVec2& inPos)>& inEntitySpawner)
{
	for (int i = 0; i < inCount; ++i)
	{
		int x = rand() % ROOM_WIDTH;
		int y = rand() % ROOM_HEIGHT;
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
	DungeonMap dungeon(ROOM_COL_COUNT, ROOM_ROW_COUNT);

	for (int col = 0; col < ROOM_COL_COUNT; ++col)
	{
		for (int row = 0; row < ROOM_ROW_COUNT; ++row)
		{
			IVec2 pos( col*ROOM_WIDTH, row*ROOM_HEIGHT );

			int doorMask = EDoorMask_None;
			doorMask |= (col != 0)					? EDoorMask_Left	: EDoorMask_None;
			doorMask |= (row != 0)					? EDoorMask_Top		: EDoorMask_None;
			doorMask |= (col != (ROOM_COL_COUNT-1)) ? EDoorMask_Right	: EDoorMask_None;
			doorMask |= (row != (ROOM_ROW_COUNT-1)) ? EDoorMask_Bottom	: EDoorMask_None;

			auto room = CreateRoom(inWorld, (EDoorMask) doorMask, pos);
			dungeon.Set(col, row, room);
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
