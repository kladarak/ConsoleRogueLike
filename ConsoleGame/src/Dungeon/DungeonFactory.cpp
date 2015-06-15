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

#include "ScreenConstants.h"

namespace DungeonFactory
{

enum
{
	ROOM_WIDTH	= ScreenConstants::EMapCols+1,
	ROOM_HEIGHT	= ScreenConstants::EMapRows,
	DOOR_WIDTH	= 6,
	DOOR_HEIGHT = 4,
	DOOR_HORIZ_OFFSET = (ROOM_WIDTH - DOOR_WIDTH) / 2,
	DOOR_VERTI_OFFSET = (ROOM_HEIGHT - DOOR_HEIGHT) / 2,

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
	'|',
};

static const char kRoomDoorHoriz[DOOR_WIDTH+1] = //+1 for terminating character...
{
	"|    |",
};

static const char kRoomDoorVerti[DOOR_HEIGHT] = 
{
	'_',
	' ',
	' ',
	'_',
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
	char scratchRoom[ROOM_HEIGHT][ROOM_WIDTH];
	memset(scratchRoom, ' ', ROOM_HEIGHT*ROOM_WIDTH);
	
	// Walls
	for (int i = 0; i < ROOM_WIDTH; ++i)	{ scratchRoom[0][i]				= kRoomWallHoriz[i]; }
	for (int i = 0; i < ROOM_WIDTH; ++i)	{ scratchRoom[ROOM_HEIGHT-1][i]	= kRoomWallHoriz[i]; }
	for (int i = 0; i < ROOM_HEIGHT; ++i)	{ scratchRoom[i][0]				= kRoomWallVerti[i]; }
	for (int i = 0; i < ROOM_HEIGHT; ++i)	{ scratchRoom[i][ROOM_WIDTH-1]	= kRoomWallVerti[i]; }

	// Doors
	if ( (inDoorMask & EDoorMask_Top) > 0 )
	{
		for (int i = 0; i < DOOR_WIDTH; ++i)	{ scratchRoom[0][DOOR_HORIZ_OFFSET + i]	= ' '; } // remove wall
		for (int i = 0; i < DOOR_WIDTH; ++i)	{ scratchRoom[1][DOOR_HORIZ_OFFSET + i]	= kRoomDoorHoriz[i]; }
	}
	
	if ( (inDoorMask & EDoorMask_Bottom) > 0 )
	{
		for (int i = 0; i < DOOR_WIDTH; ++i)	{ scratchRoom[ROOM_HEIGHT-1][DOOR_HORIZ_OFFSET + i]	= kRoomDoorHoriz[i]; }
	}
	
	if ( (inDoorMask & EDoorMask_Left) > 0 )
	{
		for (int i = 1; i < DOOR_HEIGHT; ++i)	{ scratchRoom[DOOR_VERTI_OFFSET + i][0]	= ' '; } // remove wall, but not the first bit.
		for (int i = 0; i < DOOR_HEIGHT; ++i)	{ scratchRoom[DOOR_VERTI_OFFSET + i][1]	= kRoomDoorVerti[i]; }
	}
	
	if ( (inDoorMask & EDoorMask_Right) > 0 )
	{
		for (int i = 1; i < DOOR_HEIGHT; ++i)	{ scratchRoom[DOOR_VERTI_OFFSET + i][ROOM_WIDTH-1]	= ' '; } // remove wall, but not the first bit.
		for (int i = 0; i < DOOR_HEIGHT; ++i)	{ scratchRoom[DOOR_VERTI_OFFSET + i][ROOM_WIDTH-2]	= kRoomDoorVerti[i]; }
	}

	AsciiMesh outMesh;

	for (int y = 0; y < ROOM_HEIGHT; ++y)
	{
		for (int x = 0; x < ROOM_WIDTH; ++x)
		{
			outMesh.SetCharAtPosition( x, y, scratchRoom[y][x] );
		}
	}

	//return std::move(outMesh);
	return outMesh;
}

static Entity CreateRoom(World& inWorld, EDoorMask inDoorMask, const IVec2& inPosition)
{
	Entity entity = inWorld.CreateEntity();

	entity.AddComponent<PositionComponent>( inPosition );
	entity.AddComponent<TriggerBoxComponent>( IRect(0, 0, ROOM_WIDTH, ROOM_HEIGHT) );

	auto collisionComp	= entity.AddComponent<CollisionComponent>();
	auto renderableComp	= entity.AddComponent<RenderableComponent>();
	
	AsciiMesh mesh = gGenerateRoom(inDoorMask);
	renderableComp->SetMesh(mesh);

	mesh.ForEachFrag( [&] (int inX, int inY, char inChar)
	{
		if ( kWhiteSpace.find( inChar ) == std::string::npos )
		{
			collisionComp->SetCollidableAt( inX, inY );
		}
	} );

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

std::vector<Entity> Generate(World& inWorld, MessageBroadcaster& inMessageBroadcaster)
{
	std::vector<Entity> rooms;

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
			FillRoom(room, inMessageBroadcaster);

			rooms.push_back(room);
		}
	}

	return rooms;
}

}
