#include "StateMachine.h"

StateMachine::~StateMachine()
{
	Clear();
}

void StateMachine::PopState()
{
	auto back = mStateStack.back();
	delete back;
	mStateStack.pop_back();
}

StateBase* StateMachine::GetTop() const
{
	assert(mStateStack.size() > 0);

	return mStateStack.back();
}

void StateMachine::Clear()
{
	for (auto state : mStateStack)
	{
		delete state;
	}

	mStateStack.clear();
}
