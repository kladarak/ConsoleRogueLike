#include "StartMenuState.h"

#include <Input/InputBuffer.h>

void StartMenuState::Reset()
{
}

void StartMenuState::Init()
{
}

EGameState StartMenuState::Update(float inFrameTime, const InputBuffer& inInput)
{
	return inInput.IsPressed('y') ? EGameState_InGame : EGameState_StartMenu;
}

std::string StartMenuState::GetRenderBuffer()
{
	return "Start Game?\n";
}
