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

#include "ScreenConstants.h"

namespace DungeonFactory
{

enum
{
	ROOM_WIDTH	= ScreenConstants::EMapCols+1,
	ROOM_HEIGHT	= ScreenConstants::EMapRows,
	DOOR_WIDTH	= 7,
	DOOR_HEIGHT = 5,
	DOOR_HORIZ_OFFSET = (ROOM_WIDTH - DOOR_WIDTH) / 2,
	DOOR_VERTI_OFFSET = ((ROOM_HEIGHT - DOOR_HEIGHT) / 2) + 1,

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

static const char kRoomDoorHoriz[DOOR_WIDTH+1] = //+1 for terminating character...
{
	"|     |",
};

static const char kRoomDoorVerti[DOOR_HEIGHT] = 
{
	'_',
	' ',
	' ',
	' ',
	gCastUCharToChar(238),
};

enum EDoorMask
{
	EDoorMask_None		= 0,
	EDoorMask_Top		= 1 << 0,
	EDoorMask_Bottom	= 1 << 1,
	EDoorMask_Left		= 1 << 2,
	EDoorMask_Right		= 1 << 3,
};

static AsciiMesh gGenerateRoom(EDoorMask inDoorMask)
{
	AsciiMesh mesh(' ', ROOM_HEIGHT, ROOM_WIDTH);
	
	// Walls
	for (int i = 0; i < ROOM_WIDTH; ++i)	{ mesh.SetCharAtPosition(i,				0,				kRoomWallHoriz[i]); }
	for (int i = 0; i < ROOM_WIDTH; ++i)	{ mesh.SetCharAtPosition(i,				ROOM_HEIGHT-1, 	kRoomWallHoriz[i]); }
	for (int i = 0; i < ROOM_HEIGHT; ++i)	{ mesh.SetCharAtPosition(0,				i,				kRoomWallVerti[i]); }
	for (int i = 0; i < ROOM_HEIGHT; ++i)	{ mesh.SetCharAtPosition(ROOM_WIDTH-1,	i,				kRoomWallVerti[i]); }

	// Doors
	if ( (inDoorMask & EDoorMask_Top) > 0 )
	{
		for (int i = 0; i < DOOR_WIDTH; ++i)	{ mesh.SetCharAtPosition(DOOR_HORIZ_OFFSET + i, 0, ' '); } // remove wall
		//for (int i = 0; i < DOOR_WIDTH; ++i)	{ mesh.SetCharAtPosition(DOOR_HORIZ_OFFSET + i, 1, kRoomDoorHoriz[i]); }
	}
	
	if ( (inDoorMask & EDoorMask_Bottom) > 0 )
	{
		for (int i = 0; i < DOOR_WIDTH; ++i)	{ mesh.SetCharAtPosition(DOOR_HORIZ_OFFSET + i, ROOM_HEIGHT-1, ' '); } // remove wall
		//for (int i = 0; i < DOOR_WIDTH; ++i)	{ mesh.SetCharAtPosition(DOOR_HORIZ_OFFSET + i, ROOM_HEIGHT-1, kRoomDoorHoriz[i]); }
	}
	
	if ( (inDoorMask & EDoorMask_Left) > 0 )
	{
		for (int i = 1; i < DOOR_HEIGHT-1; ++i)	{ mesh.SetCharAtPosition(0, DOOR_VERTI_OFFSET + i, ' '); } // remove wall, but not the first or last bits.
		//for (int i = 0; i < DOOR_HEIGHT; ++i)	{ mesh.SetCharAtPosition(1, DOOR_VERTI_OFFSET + i, kRoomDoorVerti[i]); }
	}
	
	if ( (inDoorMask & EDoorMask_Right) > 0 )
	{
		for (int i = 1; i < DOOR_HEIGHT-1; ++i)	{ mesh.SetCharAtPosition(ROOM_WIDTH-1, DOOR_VERTI_OFFSET + i, ' '); } // remove wall, but not the first or last bits.
		//for (int i = 0; i < DOOR_HEIGHT; ++i)	{ mesh.SetCharAtPosition(ROOM_WIDTH-2, DOOR_VERTI_OFFSET + i, kRoomDoorVerti[i]); }
	}

	return mesh;
}

static Entity CreateRoom(World& inWorld, EDoorMask inDoorMask, const IVec2& inPosition)
{
	Entity entity = inWorld.CreateEntity();

	entity.AddComponent<PositionComponent>( inPosition );
	entity.AddComponent<TriggerBoxComponent>( IRect(0, 0, ROOM_WIDTH, ROOM_HEIGHT) );

	auto collisionComp	= entity.AddComponent<CollisionComponent>();
	auto renderableComp	= entity.AddComponent<RenderableComponent>();
	
	AsciiMesh renderMesh = gGenerateRoom(inDoorMask);
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
