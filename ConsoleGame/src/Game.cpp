#include "Game.h"

#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <cstring>

#include "Windows.h"

#include "ScreenConstants.h"

#include <GameEntities/RoomEntity.h>
#include <GameEntities/Player/PlayerEntity.h>
#include <GameEntities/SpinnerEntity.h>
#include <Dungeon/DungeonFactory.h>

#include <EntityComponent/Systems/AnimationSystem.h>
#include <EntityComponent/Systems/InputHandlerSystem.h>
#include <EntityComponent/Systems/PositionSystem.h>
#include <EntityComponent/Systems/ProgramSystem.h>
#include <EntityComponent/Systems/RenderSystem.h>
#include <EntityComponent/Systems/TriggerSystem.h>

Game::Game()
	: mIsRunning(false)
{
}

int Game::Run()
{
	Init();

	while (mIsRunning)
	{
		Update();
		Render();
	}

	return 0;
}

void Game::Init()
{
	srand( (uint32_t) time(NULL) );

	mTimer.Start();
	mInputMonitor.StartMonitoring();

	auto rooms = DungeonFactory::Generate(mWorld, mMessageBroadcaster);
	
	mDungeonMap.Init(rooms);
	
	mCameraSystem.Init(mWorld, mDungeonMap);

	auto player = PlayerEntity::Create(mWorld);

	mHUD.Init(mMessageBroadcaster, player);

	mIsRunning = true;
}

void Game::Update()
{
	mTimer.Tick();
	float frameTime = mTimer.GetDeltaTime();

	mWorld.FlushDestroyedEntities();

	PositionSystem::SwapPositionBuffers(mWorld);

	InputHandlerSystem::Update(mWorld, mInputMonitor.ConsumeBuffer());

	ProgramSystem::Update(mWorld, frameTime);

	TriggerSystem::Update(mWorld);

	AnimationSystem::Update(mWorld, frameTime);
}

void Game::Render()
{
	using namespace ScreenConstants;

	COORD pos = { 0, 0 };
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(output, pos);
	
	mHUD.RenderTop();
	
	RenderTarget renderTarget(EMapCols+1, EMapRows);
	IVec2 cameraPosition = mCameraSystem.GetCameraPosition();
	RenderSystem::Render(mWorld, cameraPosition, renderTarget);

	renderTarget.Render();

	mHUD.RenderBottom();
	printf("%03i FPS (%03.3fms)           \n", (int) (1.0f / mTimer.GetDeltaTime()), mTimer.GetDeltaTime() * 1000.0f);
}
