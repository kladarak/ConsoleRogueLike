#include "DungeonFactory.h"

#include <string>
#include <EntityComponentSystem/World/World.h>

#include <EntityComponent/Components/CollisionComponent.h>
#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/RenderableComponent.h>

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
static const char kRoomWallHoriz[ROOM_WIDTH] = " ____________________________________ ";
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

AsciiMesh gGenerateRoom(EDoorMask inDoorMask)
{
	char scratchRoom[ROOM_HEIGHT][ROOM_WIDTH];
	memset(scratchRoom, ' ', ROOM_HEIGHT*ROOM_WIDTH);
	
	for (int i = 0; i < ROOM_WIDTH; ++i)	{ scratchRoom[0][i]				= kRoomWallHoriz[i]; }
	for (int i = 0; i < ROOM_WIDTH; ++i)	{ scratchRoom[ROOM_HEIGHT-1][i]	= kRoomWallHoriz[i]; }
	for (int i = 0; i < ROOM_HEIGHT; ++i)	{ scratchRoom[i][0]				= kRoomWallVerti[i]; }
	for (int i = 0; i < ROOM_HEIGHT; ++i)	{ scratchRoom[i][ROOM_WIDTH-2]	= kRoomWallVerti[i]; }

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
		for (int i = 1; i < DOOR_HEIGHT; ++i)	{ scratchRoom[DOOR_VERTI_OFFSET + i][ROOM_WIDTH-2]	= ' '; } // remove wall, but not the first bit.
		for (int i = 0; i < DOOR_HEIGHT; ++i)	{ scratchRoom[DOOR_VERTI_OFFSET + i][ROOM_WIDTH-3]	= kRoomDoorVerti[i]; }
	}

	AsciiMesh outMesh;

	for (int y = 0; y < ROOM_HEIGHT; ++y)
	{
		for (int x = 0; x < ROOM_WIDTH; ++x)
		{
			outMesh.SetCharAtPosition( IVec2(x, y), scratchRoom[y][x] );
		}
	}

	return outMesh;
}

void CreateRoom(World& inWorld, EDoorMask inDoorMask, const IVec2& inPosition)
{
	Entity entity = inWorld.CreateEntity();

	auto positionComp	= entity.AddComponent<PositionComponent>();
	auto collisionComp	= entity.AddComponent<CollisionComponent>();
	auto renderableComp	= entity.AddComponent<RenderableComponent>();

	positionComp->SetPosition( inPosition );
	
	AsciiMesh mesh = gGenerateRoom(inDoorMask);

	auto& fragments = mesh.GetFragments();
	for (auto& fragment : fragments)
	{
		if ( kWhiteSpace.find( fragment.mCharacter ) == std::string::npos )
		{
			collisionComp->SetCollidableAt( fragment.mPosition );
		}
	}

	renderableComp->SetMesh(mesh);
}

void Generate(World& inWorld)
{
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

			CreateRoom(inWorld, (EDoorMask) doorMask, pos);
		}
	}
}

}
