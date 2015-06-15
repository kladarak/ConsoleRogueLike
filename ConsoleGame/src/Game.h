#pragma once

#include <Core/Time/Timer.h>

#include <Input/InputMonitor.h>

#include <StateMachine/StateBase.h>

class Game
{
public:
	Game();
	~Game();

	int Run();

private:
	void Init();
	void HandleStateChange();
	void SwitchToState(EGameState inState);
	void Update();
	void Render();
	
	Timer				mTimer;
	InputMonitor		mInputMonitor;
	bool				mIsRunning;

	EGameState			mCurrentState;
	EGameState			mNextState;

	StateBase*			mStates[EGameState_Count];
};
