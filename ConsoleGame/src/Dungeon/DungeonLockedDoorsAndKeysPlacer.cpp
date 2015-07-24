#include "DungeonLockedDoorsAndKeysPlacer.h"

#include <algorithm>

#include <Containers/ContainerMacros.h>
#include <Renderer/Colour.h>

#include <GameEntities/ItemEntity.h>
#include <Inventory/Items/DoorKey.h>

#include "DungeonMap.h"
#include "DungeonDoorPlacer.h"
#include "DungeonGeneratorUtils.h"

namespace DungeonLockedDoorsAndKeysPlacer
{
	
enum EMode
{
	EPlaceKey,
	EPlaceLockedDoor,
};

static const EColour kColours[] = 
{
	ETextRed,
	ETextGreen,
	ETextBlue,
	ETextYellow,
};

typedef Entity (*ColouredDoorKeyItemFactoryFunc)(World& inWorld, GameData* inGameData);
static ColouredDoorKeyItemFactoryFunc kColouredKeyFactoryFuncs[] = 
{
	&ItemEntity::Create< ColouredDoorKey<ETextRed> >,
	&ItemEntity::Create< ColouredDoorKey<ETextGreen> >,
	&ItemEntity::Create< ColouredDoorKey<ETextBlue> >,
	&ItemEntity::Create< ColouredDoorKey<ETextYellow> >,
};

class LockedDoorAndKeyPlacer
{
public:
	LockedDoorAndKeyPlacer(const DungeonMap& inDungeon, World& inWorld, GameData* inGameData);

	LockedDoorAndKeyPlacer& operator=(const LockedDoorAndKeyPlacer&) = delete;

	void					DoIt(uint32_t inMaxLockedDoors);

private:
	bool					PlaceDoorAndKey(const RoomData& inRoomData, const IVec2& inLockedDoorRoomIndex, const IVec2& inKeyRoomIndex, uint32_t inKeyNum);

	const DungeonMap&		mDungeon;
	World&					mWorld;
	GameData*				mGameData;
	Dynamic2DVector<bool>	mRoomsVisited;

};

void sPlaceThem(const DungeonMap& inDungeon, World& inWorld, GameData* inGameData, uint32_t inMaxLockedDoors)
{
	LockedDoorAndKeyPlacer(inDungeon, inWorld, inGameData).DoIt(inMaxLockedDoors);
}

LockedDoorAndKeyPlacer::LockedDoorAndKeyPlacer(const DungeonMap& inDungeon, World& inWorld, GameData* inGameData)
	: mDungeon		(inDungeon)
	, mWorld		(inWorld)
	, mGameData		(inGameData)
	, mRoomsVisited	(inDungeon.GetColCount(), inDungeon.GetRowCount(), false)
{
}

void LockedDoorAndKeyPlacer::DoIt(uint32_t inMaxLockedDoors)
{
	// From player starting position, do breadth-search walk around the dungeon. Place a key first, then a matching door.

	// Start with a node at start room. Mark it as visited.
	// If in key place mode, decide if it should be placed in this room.
	// If in lock place mode, decide if a lock should be added to a door to an unvisited room.
	// If no doors into unvisited room, remove this node from the list.
	// If 1 door is unvisited, advance this node into that room, mark it as visited. // Could skip this step by always removing *this* node and adding new nodes for each door.
	// If more doors are unvisited, create additional nodes for each door and mark them all visited.

	// Limit invalid input
	inMaxLockedDoors = std::min(inMaxLockedDoors, gElemCount(kColours));
	
	std::vector<IVec2>	nodes;
	size_t				nodeIndex	= 0;
	uint32_t			locksPlaced = 0;
	EMode				mode		= EPlaceKey;
	IVec2				keyRoomIndex(-1, -1);

	{
		// Seed node list
		IVec2 startNode = mDungeon.GetPlayerStartingRoomIndex();
		nodes.push_back(startNode);
		mRoomsVisited.Set(startNode, true);
	}

	// From starting node, walk rooms in a breadth-first fashion until requested number of doors and keys have been placed, or until all rooms have been visited.
	while (nodeIndex < nodes.size() && locksPlaced < inMaxLockedDoors)
	{
		IVec2			roomIndex	= nodes[nodeIndex];
		const RoomData& roomData	= mDungeon.GetRoomData().Get( roomIndex );

		assert(roomData.mIsValid);

		// Place a key or a locked door from this room.
		if (rand() % 3 == 0)
		{
			switch (mode)
			{
				case EPlaceKey:
				{
					// Store index, but delay creating a key until a matching door is placed.
					keyRoomIndex	= roomIndex;
					mode			= EPlaceLockedDoor;
					break;
				}

				case EPlaceLockedDoor:
				{
					uint32_t keyNum = locksPlaced;
					if ( PlaceDoorAndKey(roomData, roomIndex, keyRoomIndex, keyNum) )
					{
						mode = EPlaceKey;
						++locksPlaced;
					}

					break;
				}
			}
		}
		
		// Next, create nodes for neighbouring rooms.
		for (int i = 0; i < EDoorSide_Count; ++i)
		{
			EDoorSide doorSide = (EDoorSide) i;

			if (roomData.mDoors[doorSide])
			{
				IVec2 neighbourIndex = sGetNeighbourIndex(roomIndex, doorSide);

				if (!mRoomsVisited.Get(neighbourIndex))
				{
					nodes.push_back(neighbourIndex);
					mRoomsVisited.Set(neighbourIndex, true);
				}
			}
		}

		++nodeIndex;
	}
}

bool LockedDoorAndKeyPlacer::PlaceDoorAndKey(const RoomData& inRoomData, const IVec2& inLockedDoorRoomIndex, const IVec2& inKeyRoomIndex, uint32_t inKeyNum)
{
	RoomLink	linkOptions[EDoorSide_Count];
	int			optionCount = 0;

	for (int i = 0; i < EDoorSide_Count; ++i)
	{
		EDoorSide	doorSide		= (EDoorSide) i;
		IVec2		neighbourIndex	= sGetNeighbourIndex(inLockedDoorRoomIndex, doorSide); 

		if (inRoomData.mDoors[doorSide] && !mRoomsVisited.Get(neighbourIndex))
		{
			RoomLink& link = linkOptions[optionCount];

			link.mRoomIndex0	= inLockedDoorRoomIndex;
			link.mRoomIndex1	= neighbourIndex;
			link.mRoomSide0		= doorSide;
			link.mRoomSide1		= sOppositeSide(doorSide);

			++optionCount;
		}
	}

	bool canLockADoor = (optionCount > 0);

	if (canLockADoor)
	{
		int chosenLinkIndex = rand() % optionCount;

		DungeonDoorPlacer(mWorld, mDungeon).AddLockedDoor( linkOptions[chosenLinkIndex], kColours[inKeyNum] );
						
		auto	keyEntity	= kColouredKeyFactoryFuncs[inKeyNum](mWorld, mGameData);
		IVec2	keyRoomPos	= mDungeon.GetRoomEntities().Get(inKeyRoomIndex).GetComponent<PositionComponent>()->GetPosition();
		IVec2	offsetPos	= DungeonGeneratorUtils::sGetRandomOffsetInRoom( keyEntity.GetComponent<TriggerBoxComponent>()->GetBounds() );
		IVec2	keyPos		= keyRoomPos + offsetPos;
		keyEntity.GetComponent<PositionComponent>()->SetPosition(keyPos);
	}

	return canLockADoor;
}

}
