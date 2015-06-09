#pragma once

#include <chrono>

#include <EntityComponentSystem/World/World.h>

#include <Input/InputMonitor.h>
#include <Renderer/RenderTarget.h>

class Game
{
public:
	Game();
	int Run();

private:
	void Update();
	void Render();

	typedef std::chrono::time_point<std::chrono::system_clock> SystemClockTime;
	SystemClockTime mLastClockTime;
	double			mFrameTime;
	double			mTimeElapsed;
	
	InputMonitor	mInputMonitor;
	World			mWorld;
	RenderTarget	mLastRenderedTarget;
};
