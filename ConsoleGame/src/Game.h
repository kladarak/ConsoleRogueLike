#pragma once

#include <Core/Messaging/MessageBroadcaster.h>
#include <Core/Time/Timer.h>

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
	void Init();
	void Update();
	void Render();
	
	Timer				mTimer;
	World				mWorld;
	InputMonitor		mInputMonitor;
	DungeonMap			mDungeonMap;
	CameraSystem		mCameraSystem;
	HUD					mHUD;
	MessageBroadcaster	mMessageBroadcaster;

	bool				mIsRunning;
};
