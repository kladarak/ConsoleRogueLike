#pragma once

#include <chrono>

#include <Core/Messaging/MessageBroadcaster.h>

#include <EntityComponentSystem/World/World.h>

#include <Input/InputMonitor.h>
#include <GameSystems/CameraSystem.h>
#include <Dungeon/DungeonMap.h>
#include <Renderer/RenderTarget.h>
#include <UI/HUD.h>

class Game
{
public:
	Game();

	int Run();

private:
	void Update();
	void Render();

	typedef std::chrono::time_point<std::chrono::system_clock> SystemClockTime;
	SystemClockTime		mLastClockTime;
	double				mFrameTime;
	double				mTimeElapsed;
	
	World				mWorld;
	InputMonitor		mInputMonitor;
	DungeonMap			mDungeonMap;
	CameraSystem		mCameraSystem;
	HUD					mHUD;
	MessageBroadcaster	mMessageBroadcaster;

	bool				mIsRunning;
};
