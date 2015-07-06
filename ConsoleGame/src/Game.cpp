#include "Game.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "Windows.h"

#include <StateMachine/StateBase.h>

Game::Game()
	: mStateMachineMessageHandler(mStateMachine)
	, mIsRunning(false)
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

	mIsRunning = true;
}

void Game::Update()
{
	mTimer.Tick();
	mStateMachineMessageHandler.ProcessStateChangeRequests();

	float	frameTime	= mTimer.GetDeltaTime();
	auto	inputBuffer = mInputMonitor.ConsumeBuffer();

	mStateMachine.GetTop()->Update(frameTime, inputBuffer);
}

void Game::Render()
{
	COORD	pos		= { 0, 0 };
	HANDLE	output	= GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(output, pos);

	printf("%04i FPS (%03.3fms)\n\n", (uint32_t) (1.0f / mTimer.GetDeltaTime()), mTimer.GetDeltaTime() * 1000.0f);

	std::string buffer = mStateMachine.GetTop()->GetRenderBuffer();
	printf(buffer.c_str());
}
