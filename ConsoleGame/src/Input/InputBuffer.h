#pragma once

#include <thread>

class InputBuffer
{
public:
	InputBuffer();
	~InputBuffer();
	
	void		SetPressed(char inChar);
	void		ClearPressed(char inChar);
	bool		IsPressed(char inChar) const;

private:
	bool		mKeysPressed[256];
};
