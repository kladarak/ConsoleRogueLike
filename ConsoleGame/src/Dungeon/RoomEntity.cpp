#include "RoomEntity.h"

#include <string>

#include <EntityComponent/Components/CollisionComponent.h>
#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/RenderableComponent.h>
#include <EntityComponent/Components/TriggerBoxComponent.h>

#include <Renderer/AsciiMesh.h>

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

	AsciiMesh sGenerateRoom()
	{
		AsciiMesh mesh(' ', ERoomDimensions_Height, ERoomDimensions_Width);
	
		for (int i = 0; i < ERoomDimensions_Width; ++i)		{ mesh.SetCharAtPosition(i,	0, kRoomWallHoriz[i]); }
		for (int i = 0; i < ERoomDimensions_Width; ++i)		{ mesh.SetCharAtPosition(i,	ERoomDimensions_Height-1, kRoomWallHoriz[i]); }
		for (int i = 0; i < ERoomDimensions_Height; ++i)	{ mesh.SetCharAtPosition(0,	i, kRoomWallVerti[i]); }
		for (int i = 0; i < ERoomDimensions_Height; ++i)	{ mesh.SetCharAtPosition(ERoomDimensions_Width-1, i, kRoomWallVerti[i]); }

		return mesh;
	}
}

namespace RoomEntity
{

Entity Create(World& inWorld, const IVec2& inPosition)
{
	Entity entity = inWorld.CreateEntity();

	entity.AddComponent<PositionComponent>( inPosition );
	entity.AddComponent<TriggerBoxComponent>( IRect(0, 0, ERoomDimensions_Width, ERoomDimensions_Height) );

	AsciiMesh renderMesh = sGenerateRoom();
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

void EraseWallForDoor(Entity inRoom, EDoorSide inSide)
{
	struct DoorEraseSetup
	{
		int		mStart;
		int		mEnd;
		IVec2	mPosition;
		IVec2	mOffset;
	};

	static const DoorEraseSetup kDoorEraseSetup[] =
	{
		{ 0, EDoorSize_Width,		IVec2(ERoomDimensions_DoorHorizOffset,	0),									IVec2(1, 0) },	 // EDoorSide_Top
		{ 0, EDoorSize_Width,		IVec2(ERoomDimensions_DoorHorizOffset,	ERoomDimensions_Height - 1),		IVec2(1, 0) },	 // EDoorSide_Bottom
		{ 1, EDoorSize_Height - 1,	IVec2(0,								ERoomDimensions_DoorVertiOffset),	IVec2(0, 1) },	 // EDoorSide_Left
		{ 1, EDoorSize_Height - 1,	IVec2(ERoomDimensions_Width - 1,		ERoomDimensions_DoorVertiOffset),	IVec2(0, 1) },	 // EDoorSide_Right
	};

	const DoorEraseSetup& setup = kDoorEraseSetup[inSide];

	auto renderable = inRoom.GetComponent<RenderableComponent>();
	auto collision	= inRoom.GetComponent<CollisionComponent>();

	for (int i = setup.mStart; i < setup.mEnd; ++i)
	{
		IVec2 pos = setup.mPosition + (setup.mOffset * i);

		renderable->SetCharAtPosition(pos.mX, pos.mY, ' ');
		collision->ClearDefaultMeshCollidableAt(pos.mX, pos.mY);
	}
}

}