#pragma once

#include <Core/Time/Timer.h>

#include <Input/InputMonitor.h>

#include <StateMachine/InGameState.h>

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
	InputMonitor		mInputMonitor;
	bool				mIsRunning;

	InGameState			mInGameState;
};
