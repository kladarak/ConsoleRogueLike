#pragma once

#include <Maths/IVec2.h>
#include <EntityComponentSystem/World/World.h>

class DungeonMap;

class CameraSystem
{
public:
	CameraSystem();

	void	Init(World& inWorld, const DungeonMap& inDungeonMap);
	
	IVec2	GetCameraPosition() const;

private:
	void	OnEntityEnteredRoom(const Entity& inRoom, const Entity& inTriggerer);

	Entity	mCameraEntity;

};
