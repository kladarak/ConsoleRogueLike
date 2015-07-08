#pragma once

#include <vector>
#include <Maths/IVec2.h>

#include "RoomEntity.h"
#include "DungeonMap.h"
#include "DungeonLayoutGenerator.h"

class World;
class MessageBroadcaster;

struct RoomLink
{
	IVec2		mRoomIndex0;
	IVec2		mRoomIndex1;
	EDoorSide	mRoomSide0;
	EDoorSide	mRoomSide1;
};

class DungeonDoorPlacer
{
public:
	static std::vector<RoomLink> sGenerateRoomLinks(const DungeonLayout& inLayout);

	DungeonDoorPlacer(World& inWorld, DungeonMap& inDungeonMap);

	void		AddOpenDoor(const RoomLink& inRoomLink);
	void		AddLockedDoor(const RoomLink& inRoomLink);
	void		AddDungeonExit(const IVec2& inRoomIndex, MessageBroadcaster& inMessageBroadcaster);

private:
	World&		mWorld;
	DungeonMap&	mDungeonMap;
};