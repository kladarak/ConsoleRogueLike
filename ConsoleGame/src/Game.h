#pragma once

#include <Core/Time/Timer.h>
#include <Input/InputMonitor.h>
#include <StateMachine/StateMachine.h>
#include <StateMachine/StateMachineMessageHandler.h>

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
	StateMachine				mStateMachine;
	StateMachineMessageHandler	mStateMachineMessageHandler;
	bool						mIsRunning;
};
