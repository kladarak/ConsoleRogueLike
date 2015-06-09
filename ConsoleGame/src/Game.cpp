#include "Game.h"

#include <cstdio>
#include <stdlib.h>
#include <chrono>
#include <cstring>

#include "ScreenConstants.h"

#include <GameEntities/RoomEntity.h>
#include <GameEntities/PlayerEntity.h>
#include <GameEntities/SpinnerEntity.h>

#include <EntityComponent/Systems/InputHandlerSystem.h>
#include <EntityComponent/Systems/ProgramSystem.h>
#include <EntityComponent/Systems/RenderSystem.h>

Game::Game()
	: mLastClockTime		( std::chrono::system_clock::now() )
	, mFrameTime			( 0.0 )
	, mTimeElapsed			( 0.0 )
	, mLastRenderedTarget	( ScreenConstants::EMapCols, ScreenConstants::EMapRows )
{
}

int Game::Run()
{
	mLastClockTime = std::chrono::system_clock::now();
	mInputMonitor.StartMonitoring();

	RoomEntity::Create(mWorld);
	PlayerEntity::Create(mWorld);
	SpinnerEntity::Create(mWorld, IVec2(20, 10));
	SpinnerEntity::Create(mWorld, IVec2(20, 5));
	SpinnerEntity::Create(mWorld, IVec2(10, 5));

	while (true)
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

	InputBuffer inputBuffer = mInputMonitor.ConsumeBuffer();
	InputHandlerSystem::Update(&mWorld, inputBuffer);

	ProgramSystem::Update(&mWorld, (float) mFrameTime);
}

void Game::Render()
{
	using namespace ScreenConstants;

	RenderTarget renderTarget(EMapCols, EMapRows);
	RenderSystem::Render(&mWorld, renderTarget);

	if (mLastRenderedTarget != renderTarget)
	{
		system("cls");
		renderTarget.Render();
		mLastRenderedTarget = renderTarget;
	}
}
