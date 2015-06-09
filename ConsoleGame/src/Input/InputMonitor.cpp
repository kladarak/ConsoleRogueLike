#include "InputMonitor.h"

#include <conio.h>

InputMonitor::InputMonitor()
	: mUpdating(false)
{
}

InputMonitor::~InputMonitor()
{
	if (mUpdating)
	{
		mUpdating = false;
		mThread.join();
	}
}

void InputMonitor::StartMonitoring()
{
	mUpdating = true;
	mThread = std::thread(&InputMonitor::sTrampolineInput, this);
}

void InputMonitor::sTrampolineInput(InputMonitor* inInput)
{
	inInput->MonitorInput();
}

void InputMonitor::MonitorInput()
{
	while (mUpdating)
	{
		int c = _getch();

		mMutex.lock();
		mBuffer.SetPressed( (char) c );
		mMutex.unlock();
	}
}

InputBuffer	InputMonitor::ConsumeBuffer()
{
	mMutex.lock();
	InputBuffer copy = mBuffer;
	mBuffer = InputBuffer();
	mMutex.unlock();

	return copy;
}
