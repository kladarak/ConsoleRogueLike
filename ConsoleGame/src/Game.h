#pragma once

#include <Core/Time/Timer.h>
#include <Input/InputMonitor.h>
#include <StateMachine/StateMachine.h>
#include <StateMachine/StateMachineMessageHandler.h>

#include "GameData.h"

class Game
{
public:
	Game();

	int							Run();

private:
	void						Init();
	void						Update();
	void						Render();
	
	Timer						mTimer;
	InputMonitor				mInputMonitor;
	GameData					mGameData;
	StateMachine				mStateMachine;
	StateMachineMessageHandler	mStateMachineMessageHandler;
	bool						mIsRunning;
};
