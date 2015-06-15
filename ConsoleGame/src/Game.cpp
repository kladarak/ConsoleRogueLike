#include "Game.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "Windows.h"

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

	mInGameState.Init();

	mIsRunning = true;
}

void Game::Update()
{
	mTimer.Tick();
	float frameTime = mTimer.GetDeltaTime();
	auto inputBuffer = mInputMonitor.ConsumeBuffer();

	mInGameState.Update(frameTime, inputBuffer);
}

void Game::Render()
{
	std::string buffer = mInGameState.GetRenderBuffer();

	COORD pos = { 0, 0 };
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(output, pos);
	
	printf(buffer.c_str());

	printf("%04i FPS (%03.3fms)           \n", (uint32_t) (1.0f / mTimer.GetDeltaTime()), mTimer.GetDeltaTime() * 1000.0f);
}
