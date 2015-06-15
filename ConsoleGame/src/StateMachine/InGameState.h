#pragma once

#include <Core/Messaging/MessageBroadcaster.h>

#include <EntityComponentSystem/World/World.h>

#include <GameSystems/CameraSystem.h>
#include <Dungeon/DungeonMap.h>
#include <UI/HUD.h>

class InputBuffer;

class InGameState
{
public:
	void		Init();
	void		Update(float inFrameTime, const InputBuffer& inInput);
	std::string GetRenderBuffer();

private:
	World				mWorld;
	DungeonMap			mDungeonMap;
	CameraSystem		mCameraSystem;
	HUD					mHUD;
	MessageBroadcaster	mMessageBroadcaster;

};
