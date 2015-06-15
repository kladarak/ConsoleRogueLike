#include "Game.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "Windows.h"

#include <StateMachine/StartMenuState.h>
#include <StateMachine/InGameState.h>

Game::Game()
	: mIsRunning(false)
	, mCurrentState(EGameState_StartMenu)
	, mNextState(EGameState_StartMenu)
{
	memset(mStates, 0, sizeof(mStates));
}

Game::~Game()
{
	for (int i = 0; i < EGameState_Count; ++i)
	{
		delete mStates[i];
	}
}

int Game::Run()
{
	Init();

	while (mIsRunning)
	{
		HandleStateChange();
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
	
	mStates[EGameState_StartMenu]	= new StartMenuState();
	mStates[EGameState_InGame]		= new InGameState();

	SwitchToState(EGameState_StartMenu);

	mIsRunning = true;
}

void Game::HandleStateChange()
{
	if (mNextState != mCurrentState)
	{
		SwitchToState(mNextState);
		system("cls");
	}
}

void Game::SwitchToState(EGameState inState)
{
	mNextState = mCurrentState = inState;

	mStates[mCurrentState]->Reset();
	mStates[mCurrentState]->Init();
}

void Game::Update()
{
	mTimer.Tick();
	float frameTime = mTimer.GetDeltaTime();
	auto inputBuffer = mInputMonitor.ConsumeBuffer();

	mNextState = mStates[mCurrentState]->Update(frameTime, inputBuffer);
}

void Game::Render()
{
	std::string buffer = mStates[mCurrentState]->GetRenderBuffer();

	COORD pos = { 0, 0 };
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(output, pos);
	
	printf("%04i FPS (%03.3fms)           \n\n", (uint32_t) (1.0f / mTimer.GetDeltaTime()), mTimer.GetDeltaTime() * 1000.0f);
	printf(buffer.c_str());

}
