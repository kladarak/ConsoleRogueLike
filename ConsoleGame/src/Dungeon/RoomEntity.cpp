#include "RoomEntity.h"

#include <string>

#include <EntityComponent/Components/CollisionComponent.h>
#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/RenderableComponent.h>
#include <EntityComponent/Components/TriggerBoxComponent.h>

#include <Renderer/AsciiMesh.h>

#include "WallFragments.h"
#include "ScreenConstants.h"

namespace
{
	const std::string	kWhiteSpace = " \n\r\t\0";

	AsciiMesh sGenerateRoom()
	{
		using namespace DungeonWall;

		AsciiMesh mesh(' ', ERoomDimensions_Height, ERoomDimensions_Width);
	
		for (int i = 1; i < ERoomDimensions_Width-1; ++i)	{ mesh.Set(i,	0, kHorizontalWall); }
		for (int i = 1; i < ERoomDimensions_Width-1; ++i)	{ mesh.Set(i,	ERoomDimensions_Height-1, kHorizontalWall); }
		for (int i = 1; i < ERoomDimensions_Height-1; ++i)	{ mesh.Set(0,	i, kVerticalWall); }
		for (int i = 1; i < ERoomDimensions_Height-1; ++i)	{ mesh.Set(ERoomDimensions_Width-1, i, kVerticalWall); }
		
		mesh.Set(0,							0,							kTopLeftCorner);
		mesh.Set(ERoomDimensions_Width-1,	0,							kTopRightCorner);
		mesh.Set(0,							ERoomDimensions_Height-1,	kBottomLeftCorner);
		mesh.Set(ERoomDimensions_Width-1,	ERoomDimensions_Height-1,	kBottomRightCorner);

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

	renderMesh.ForEachFrag( [&] (int inX, int inY, const Fragment& inFrag)
	{
		if ( kWhiteSpace.find( inFrag.mChar ) == std::string::npos )
		{
			collisionMesh.SetCollidableAt( inX, inY );
		}
	} );

	entity.AddComponent<CollisionComponent>(collisionMesh);

	return entity;
}

void EraseWallForDoor(Entity inRoom, EDoorSide inSide)
{
	using namespace DungeonWall;

	struct DoorEraseSetup
	{
		int			mStart;
		int			mEnd;
		IVec2		mPosition;
		IVec2		mOffset;
		Fragment	mCorner0;
		Fragment	mCorner1;
	};

	static const DoorEraseSetup kDoorEraseSetup[] =
	{
		{ 0, EDoorSize_Width,	IVec2(ERoomDimensions_DoorHorizOffset,	0),									IVec2(1, 0), kTopRightCorner,		kTopLeftCorner		},	 // EDoorSide_Top
		{ 0, EDoorSize_Width,	IVec2(ERoomDimensions_DoorHorizOffset,	ERoomDimensions_Height - 1),		IVec2(1, 0), kBottomRightCorner,	kBottomLeftCorner	},	 // EDoorSide_Bottom
		{ 0, EDoorSize_Height,	IVec2(0,								ERoomDimensions_DoorVertiOffset),	IVec2(0, 1), kBottomLeftCorner,		kTopLeftCorner		},	 // EDoorSide_Left
		{ 0, EDoorSize_Height,	IVec2(ERoomDimensions_Width - 1,		ERoomDimensions_DoorVertiOffset),	IVec2(0, 1), kBottomRightCorner,	kTopRightCorner		},	 // EDoorSide_Right
	};

	const DoorEraseSetup& setup = kDoorEraseSetup[inSide];

	auto renderable = inRoom.GetComponent<RenderableComponent>();
	auto collision	= inRoom.GetComponent<CollisionComponent>();

	for (int i = setup.mStart; i < setup.mEnd; ++i)
	{
		IVec2 pos = setup.mPosition + (setup.mOffset * i);

		if (i == setup.mStart)
		{
			renderable->SetFragAtPosition(pos.mX, pos.mY, setup.mCorner0);
		}
		else if (i == setup.mEnd - 1)
		{
			renderable->SetFragAtPosition(pos.mX, pos.mY, setup.mCorner1);
		}
		else
		{
			renderable->SetCharAtPosition(pos.mX, pos.mY, ' ');
			collision->ClearDefaultMeshCollidableAt(pos.mX, pos.mY);
		}
	}
}

}