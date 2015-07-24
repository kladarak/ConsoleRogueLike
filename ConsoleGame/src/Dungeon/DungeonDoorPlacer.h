#pragma once

#include <vector>
#include <Maths/IVec2.h>

#include <Renderer/Colour.h>

#include "DungeonMap.h"

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
	static std::vector<RoomLink> sGenerateRoomLinks(const DungeonMap& inLayout);

	DungeonDoorPlacer(World& inWorld, const DungeonMap& inDungeonMap);

	DungeonDoorPlacer&	operator=(const DungeonDoorPlacer& inOther) = delete;

	void				AddOpenDoor(const RoomLink& inRoomLink);
	void				AddLockedDoor(const RoomLink& inRoomLink, EColour inColour);
	void				AddDungeonExit(const IVec2& inRoomIndex, MessageBroadcaster& inMessageBroadcaster);

private:
	const DungeonMap&	mDungeonMap;
	World&				mWorld;
};