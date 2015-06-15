#pragma once

#include <string>

#include "EGameState.h"

class InputBuffer;

class StateBase
{
public:
	virtual ~StateBase() { }
	
	virtual void		Reset()													= 0;
	virtual void		Init()													= 0;
	virtual EGameState	Update(float inFrameTime, const InputBuffer& inInput)	= 0;
	virtual	std::string GetRenderBuffer()										= 0;
};
