#pragma once

#include "StateBase.h"

class StartMenuState : public StateBase
{
public:
	StartMenuState();

	virtual void		Reset();
	virtual void		Init();
	virtual EGameState	Update(float inFrameTime, const InputBuffer& inInput);
	virtual	std::string GetRenderBuffer();

private:
	int		mHighlightedMenuOption;
};
