#pragma once

#include <Maths/IVec2.h>
#include <EntityComponentSystem/World/World.h>
#include <Dungeon/DungeonMap.h>

class CameraSystem
{
public:
	CameraSystem();

	void	Init(World& inWorld, DungeonMap& inDungeonMap);
	
	IVec2	GetCameraPosition() const;

private:
	void	MoveCameraToRoom(const Entity& inRoom);

	Entity	mCameraEntity;

};
