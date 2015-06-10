#include "Game.h"

#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <cstring>

#include "ScreenConstants.h"

#include <GameEntities/RoomEntity.h>
#include <GameEntities/PlayerEntity.h>
#include <GameEntities/SpinnerEntity.h>
#include <Dungeon/DungeonFactory.h>

#include <EntityComponent/Systems/InputHandlerSystem.h>
#include <EntityComponent/Systems/ProgramSystem.h>
#include <EntityComponent/Systems/RenderSystem.h>
#include <EntityComponent/Systems/TriggerSystem.h>

Game::Game()
	: mLastClockTime		( std::chrono::system_clock::now() )
	, mFrameTime			( 0.0 )
	, mTimeElapsed			( 0.0 )
	, mLastRenderedTarget	( ScreenConstants::EMapCols+1, ScreenConstants::EMapRows )
	, mIsRunning(false)
{
}

int Game::Run()
{
	srand( (uint32_t) time(NULL) );

	mLastClockTime = std::chrono::system_clock::now();

	mInputMonitor.StartMonitoring();
	
	mHUD.Init(mMessageBroadcaster);

	auto rooms = DungeonFactory::Generate(mWorld, mMessageBroadcaster);
	mDungeonMap.Init(rooms);
	
	mCameraSystem.Init(mWorld, mDungeonMap);

	PlayerEntity::Create(mWorld);
	SpinnerEntity::Create(mWorld, IVec2(20, 10));
	SpinnerEntity::Create(mWorld, IVec2(20, 5));
	SpinnerEntity::Create(mWorld, IVec2(10, 5));

	mIsRunning = true;
	while (mIsRunning)
	{
		Update();
		Render();
	}

	return 0;
}

void Game::Update()
{
	typedef std::chrono::duration<double> ChronoDuration;
	SystemClockTime now			= std::chrono::system_clock::now();
	ChronoDuration	elapsedSec	= now - mLastClockTime;
	
	mLastClockTime	= now;
	mFrameTime		= elapsedSec.count();
	mTimeElapsed	+= mFrameTime;

	mWorld.FlushDestroyedEntities();

	InputBuffer inputBuffer = mInputMonitor.ConsumeBuffer();
	InputHandlerSystem::Update(mWorld, inputBuffer);

	TriggerSystem::Update(mWorld);

	ProgramSystem::Update(mWorld, (float) mFrameTime);
}

void Game::Render()
{
	using namespace ScreenConstants;

	RenderTarget renderTarget(EMapCols+1, EMapRows);
	IVec2 cameraPosition = mCameraSystem.GetCameraPosition();
	RenderSystem::Render(mWorld, cameraPosition, renderTarget);

	if (mLastRenderedTarget != renderTarget || mHUD.NeedsRefreshing())
	{
		system("cls");
		renderTarget.Render();
		mLastRenderedTarget = renderTarget;

		mHUD.Render();
	}
}
