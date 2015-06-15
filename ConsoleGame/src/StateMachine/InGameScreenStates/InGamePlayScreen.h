#pragma once

#include <Core/Messaging/MessageBroadcaster.h>
#include <EntityComponentSystem/World/World.h>
#include <GameSystems/CameraSystem.h>
#include <Dungeon/DungeonMap.h>
#include <UI/HUD.h>

class InputBuffer;

class InGamePlayScreen
{
public:
	void				Init();
	void				Update(float inFrameTime, const InputBuffer& inInput);
	std::string			GetRenderBuffer();

	Entity				GetPlayer() const { return mPlayer; }

private:
	World				mWorld;
	DungeonMap			mDungeonMap;
	CameraSystem		mCameraSystem;
	HUD					mHUD;
	MessageBroadcaster	mMessageBroadcaster;
	Entity				mPlayer;
};
