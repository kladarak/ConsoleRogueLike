#include "InputBuffer.h"


InputBuffer::InputBuffer()
{
	memset(mKeysPressed, 0, sizeof(mKeysPressed));
}

InputBuffer::~InputBuffer()
{
}

void InputBuffer::SetPressed(char inChar)
{
	mKeysPressed[inChar] = true;
}

void InputBuffer::ClearPressed(char inChar)
{
	mKeysPressed[inChar] = false;
}

bool InputBuffer::IsPressed(char inChar) const
{
	return mKeysPressed[inChar];
}
