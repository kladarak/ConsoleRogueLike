#pragma once

#include <thread>
#include <mutex>

#include "InputBuffer.h"

class InputMonitor
{
public:
	InputMonitor();
	~InputMonitor();

	void		StartMonitoring();

	InputBuffer	ConsumeBuffer();

private:
	static void sTrampolineInput(InputMonitor* inInput);

	void		MonitorInput();
	
	std::thread mThread;
	std::mutex	mMutex;
	InputBuffer	mBuffer;
	bool		mUpdating;
};
