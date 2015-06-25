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
#include <GameEntities/Obstacles/OpenDoor.h>
#include <GameEntities/Obstacles/DoorConstants.h>

#include "RoomEntity.h"
#include "ScreenConstants.h"
#include "DungeonLayoutGenerator.h"

namespace DungeonFactory
{

enum
{
	ROOM_COL_COUNT = 3,
	ROOM_ROW_COUNT = 3,
};

struct RoomLink
{
	IVec2		mRoomIndex0;
	IVec2		mRoomIndex1;
	EDoorSide	mRoomSide0;
	EDoorSide	mRoomSide1;
};

static std::vector<RoomLink> sGenerateRoomLinks(const DungeonLayout& inLayout)
{
	std::vector<RoomLink> links;
	
	inLayout.ForEach( [&] (size_t inCol, size_t inRow, const RoomData& inRoomData)
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
	
	inLayout.ForEach( [&] (size_t inCol, size_t inRow, const RoomData& inRoomData)
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
	DungeonLayout layout = DungeonLayoutGenerator(25).Generate();

	DungeonMap dungeon;

	layout.ForEach( [&] (size_t inCol, size_t inRow, const RoomData& inRoomData)
	{
		if (inRoomData.mIsValid)
		{
			IVec2 pos( inCol * ERoomDimensions_Width, inRow * ERoomDimensions_Height );
			auto room = RoomEntity::Create(inWorld, pos);
			dungeon.Set(inCol, inRow, room);
		}
	} );
	
	struct DoorConstructInfo
	{
		IVec2			mLocalPosition;
		EOrientation	mOrientation;
	};

	static const DoorConstructInfo kDoorConstructInfo[] =
	{
		{ IVec2(ERoomDimensions_DoorHorizOffset,	1),									EOrientation_FaceUp		},
		{ IVec2(ERoomDimensions_DoorHorizOffset,	ERoomDimensions_Height-1),			EOrientation_FaceDown	},
		{ IVec2(1,									ERoomDimensions_DoorVertiOffset),	EOrientation_FaceLeft	},
		{ IVec2(ERoomDimensions_Width-2,			ERoomDimensions_DoorVertiOffset),	EOrientation_FaceRight	},
	};
	
	// Addd doors
	{
		auto constructDoor = [&] (const IVec2& inRoomIndex, EDoorSide inSide)
		{
			auto	room			= dungeon.Get(inRoomIndex.mX, inRoomIndex.mY);
			auto&	roomPos			= room.GetComponent<PositionComponent>()->GetPosition();
			auto&	constructInfo	= kDoorConstructInfo[inSide];
			auto	doorPos			= roomPos + constructInfo.mLocalPosition;

			RoomEntity::EraseWallForDoor(room, inSide);
			return OpenDoor::Create(inWorld, doorPos, constructInfo.mOrientation);
		};

		auto doorLinks = sGenerateRoomLinks(layout);

		for (auto& link : doorLinks)
		{
			constructDoor(link.mRoomIndex0, link.mRoomSide0);
			constructDoor(link.mRoomIndex1, link.mRoomSide1);
		}
	}

	// Fill rooms after creating all rooms, to fix draw order problems.
	// A better way would be to either have an initial clearing screen pass then skip rendering any whitespace,
	// or render white space only if no other character has been written to a fragment yet,
	// or sort the renderables based on some sort of mark up on each renderable.
	dungeon.ForEach( [&] (size_t, size_t, const Entity& inRoom)
	{
		if (inRoom.IsValid())
		{
			FillRoom(inRoom, inMessageBroadcaster);
		}
	} );

	return dungeon;
}

}
