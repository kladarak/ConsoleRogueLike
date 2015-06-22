#include "RoomEntityBuilder.h"

#include <string>

#include <EntityComponent/Components/CollisionComponent.h>
#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/RenderableComponent.h>
#include <EntityComponent/Components/TriggerBoxComponent.h>

#include <GameEntities/Obstacles/DoorConstants.h>

#include "ScreenConstants.h"

namespace
{
	const std::string	kWhiteSpace = " \n\r\t\0";
	const char			kRoomWallHoriz[ERoomDimensions_Width+1] = " _____________________________________ "; //+1 for terminating character...
	const char			kRoomWallVerti[ERoomDimensions_Height] =
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
}

RoomEntityBuilder::RoomEntityBuilder(World& inWorld)
	: mWorld	(inWorld)
	, mDoorMask	(EDoorMask_None)
{
}

RoomEntityBuilder& RoomEntityBuilder::SetDoorMask(EDoorMask inMask)
{
	mDoorMask = inMask;
	return *this;
}

RoomEntityBuilder& RoomEntityBuilder::SetPosition(const IVec2& inPosition)
{
	mPosition = inPosition;
	return *this;
}

Entity RoomEntityBuilder::Create() const
{
	Entity entity = mWorld.CreateEntity();

	entity.AddComponent<PositionComponent>( mPosition );
	entity.AddComponent<TriggerBoxComponent>( IRect(0, 0, ERoomDimensions_Width, ERoomDimensions_Height) );

	AsciiMesh renderMesh = sGenerateRoom();
	sSetupDoors(renderMesh, mDoorMask);
	entity.AddComponent<RenderableComponent>(renderMesh);

	CollisionMesh collisionMesh;

	renderMesh.ForEachFrag( [&] (int inX, int inY, char inChar)
	{
		if ( kWhiteSpace.find( inChar ) == std::string::npos )
		{
			collisionMesh.SetCollidableAt( inX, inY );
		}
	} );

	entity.AddComponent<CollisionComponent>(collisionMesh);

	return entity;
}


AsciiMesh RoomEntityBuilder::sGenerateRoom()
{
	AsciiMesh mesh(' ', ERoomDimensions_Height, ERoomDimensions_Width);
	
	for (int i = 0; i < ERoomDimensions_Width; ++i)		{ mesh.SetCharAtPosition(i,	0, kRoomWallHoriz[i]); }
	for (int i = 0; i < ERoomDimensions_Width; ++i)		{ mesh.SetCharAtPosition(i,	ERoomDimensions_Height-1, kRoomWallHoriz[i]); }
	for (int i = 0; i < ERoomDimensions_Height; ++i)	{ mesh.SetCharAtPosition(0,	i, kRoomWallVerti[i]); }
	for (int i = 0; i < ERoomDimensions_Height; ++i)	{ mesh.SetCharAtPosition(ERoomDimensions_Width-1, i, kRoomWallVerti[i]); }

	return mesh;
}

void RoomEntityBuilder::sSetupDoors(AsciiMesh& inMesh, EDoorMask inDoorMask)
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

	clearPositions(EDoorMask_Top,		EDoorSize_Width,	[] (size_t i) { return IVec2(ERoomDimensions_DoorHorizOffset + i, 0); } );
	clearPositions(EDoorMask_Bottom,	EDoorSize_Width,	[] (size_t i) { return IVec2(ERoomDimensions_DoorHorizOffset + i, ERoomDimensions_Height-1); } );
	clearPositions(EDoorMask_Left,		EDoorSize_Height-2, [] (size_t i) { return IVec2(0, ERoomDimensions_DoorVertiOffset+i+1); } );
	clearPositions(EDoorMask_Right,		EDoorSize_Height-2, [] (size_t i) { return IVec2(ERoomDimensions_Width-1, ERoomDimensions_DoorVertiOffset+i+1); } );
}
