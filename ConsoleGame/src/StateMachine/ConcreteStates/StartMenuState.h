#pragma once

#include <stdint.h>
#include <StateMachine/StateBase.h>

class StartMenuState : public StateBase
{
public:
	StartMenuState();
	virtual ~StartMenuState() { }

	virtual void		Update(float inFrameTime, const InputBuffer& inInput);
	virtual	std::string GetRenderBuffer() const;

private:
	virtual void		Init();

	uint32_t			mHighlightedMenuOption;
};
