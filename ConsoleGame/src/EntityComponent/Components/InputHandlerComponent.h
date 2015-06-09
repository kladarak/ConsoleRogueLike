#pragma once

#include <functional>
#include <vector>

class Entity;
class InputBuffer;

class InputHandlerComponent
{
public:
	InputHandlerComponent()		{ }
	~InputHandlerComponent()	{ }
	InputHandlerComponent(InputHandlerComponent&& inRHS)		: mHandlers( std::move(inRHS.mHandlers) )	{ }
	InputHandlerComponent(const InputHandlerComponent& inRHS)	: mHandlers( inRHS.mHandlers )				{ }

	typedef std::function<void (const Entity& inThis, const InputBuffer&)> InputHandler;
	void								RegisterHandler(const InputHandler& inHandler)	{ mHandlers.push_back(inHandler); }
	
	const std::vector<InputHandler>&	GetHandlers() const								{ return mHandlers; }
private:
	std::vector<InputHandler>			mHandlers;
};
