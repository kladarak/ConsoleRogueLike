#pragma once

#include <EntityComponentSystem/World/World.h>

#include <vector>

class DungeonMap
{
public:
	void Init(const std::vector<Entity>& inRooms);
	
	const std::vector<Entity>& GetRooms() const { return mRooms; }

private:
	std::vector<Entity> mRooms;

};
