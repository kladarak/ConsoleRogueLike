#include "DungeonDoorPlacer.h"

#include <functional>

#include <EntityComponent/Components/EOrientation.h>
#include <EntityComponent/Components/PositionComponent.h>

#include <GameEntities/Obstacles/DoorConstants.h>
#include <GameEntities/Obstacles/LockedDoor.h>
#include <GameEntities/Obstacles/OpenDoor.h>
#include <GameEntities/Obstacles/Stairs.h>

#include "DungeonMap.h"
#include "RoomEntity.h"

namespace
{
	struct DoorConstructInfo
	{
		IVec2			mLocalPosition;
		EOrientation	mOrientation;
	};

	const DoorConstructInfo kDoorConstructInfo[] =
	{
		{ IVec2(ERoomDimensions_DoorHorizOffset,	1),									EOrientation_FaceUp		},
		{ IVec2(ERoomDimensions_DoorHorizOffset,	ERoomDimensions_Height-2),			EOrientation_FaceDown	},
		{ IVec2(1,									ERoomDimensions_DoorVertiOffset),	EOrientation_FaceLeft	},
		{ IVec2(ERoomDimensions_Width-2,			ERoomDimensions_DoorVertiOffset),	EOrientation_FaceRight	},
	};

	typedef const std::function<Entity (const IVec2&, EOrientation)>& DoorConstructFunc;

	template<typename TDoorConstructFunc>
	Entity ConstructDoor(const DungeonMap& inDungeonMap, const IVec2& inRoomIndex, EDoorSide inSide, const TDoorConstructFunc& inFunc)
	{
		auto	room			= inDungeonMap.GetRoomEntities().Get(inRoomIndex.mX, inRoomIndex.mY);
		auto&	roomPos			= room.GetComponent<PositionComponent>()->GetPosition();
		auto&	constructInfo	= kDoorConstructInfo[inSide];
		auto	doorPos			= roomPos + constructInfo.mLocalPosition;

		RoomEntity::EraseWallForDoor(room, inSide);
		return inFunc(doorPos, constructInfo.mOrientation);
	}
}

std::vector<RoomLink> DungeonDoorPlacer::sGenerateRoomLinks(const DungeonMap& inDungeon)
{
	std::vector<RoomLink> links;
	
	inDungeon.GetRoomData().ForEach( [&] (size_t inCol, size_t inRow, const RoomData& inRoomData)
	{
		if (inRoomData.mDoors[EDoorSide_Bottom])
		{
			RoomLink link;
			link.mRoomIndex0	= IVec2(inCol, inRow);
			link.mRoomIndex1	= IVec2(inCol, inRow+1);
			link.mRoomSide0		= EDoorSide_Bottom;
			link.mRoomSide1		= EDoorSide_Top;
			links.push_back(link);
		}
	} );
	
	inDungeon.GetRoomData().ForEach( [&] (size_t inCol, size_t inRow, const RoomData& inRoomData)
	{
		if (inRoomData.mDoors[EDoorSide_Right])
		{
			RoomLink link;
			link.mRoomIndex0	= IVec2(inCol, inRow);
			link.mRoomIndex1	= IVec2(inCol+1, inRow);
			link.mRoomSide0		= EDoorSide_Right;
			link.mRoomSide1		= EDoorSide_Left;
			links.push_back(link);
		}
	} );

	return links;
}

DungeonDoorPlacer::DungeonDoorPlacer(World& inWorld, const DungeonMap& inDungeonMap)
	: mDungeonMap	(inDungeonMap)
	, mWorld		(inWorld)
{
}

void DungeonDoorPlacer::AddOpenDoor(const RoomLink& inRoomLink)
{
	auto constructOpenDoor = [&] (const IVec2& inPosition, EOrientation inOrientation) { return OpenDoor::Create(mWorld, inPosition, inOrientation); };

	ConstructDoor(mDungeonMap, inRoomLink.mRoomIndex0, inRoomLink.mRoomSide0, constructOpenDoor);
	ConstructDoor(mDungeonMap, inRoomLink.mRoomIndex1, inRoomLink.mRoomSide1, constructOpenDoor);
}

void DungeonDoorPlacer::AddLockedDoor(const RoomLink& inRoomLink, EColour inColour)
{
	auto constructLockedDoor = [&] (const IVec2& inPosition, EOrientation inOrientation) { return LockedDoor::Create(mWorld, inPosition, inOrientation, inColour); };
	
	auto door0 = ConstructDoor(mDungeonMap, inRoomLink.mRoomIndex0, inRoomLink.mRoomSide0, constructLockedDoor);
	auto door1 = ConstructDoor(mDungeonMap, inRoomLink.mRoomIndex1, inRoomLink.mRoomSide1, constructLockedDoor);

	LockedDoor::BindDoors(door0, door1);
}
	
void DungeonDoorPlacer::AddDungeonExit(const IVec2& inRoomIndex, MessageBroadcaster& inMessageBroadcaster)
{
	ConstructDoor( mDungeonMap, inRoomIndex, EDoorSide_Top, [&] (const IVec2& inPosition, EOrientation inOrientation) 
	{
		LockedDoor::Create(mWorld, inPosition, inOrientation, ETextDarkYellow);
		IVec2 stairsPos(inPosition.mX, inPosition.mY-1);
		return Stairs::Create(mWorld, inMessageBroadcaster, stairsPos);
	} );
}
