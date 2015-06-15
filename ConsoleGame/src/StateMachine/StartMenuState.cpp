#include "StartMenuState.h"

#include <Containers/ContainerMacros.h>

#include <Input/InputBuffer.h>


enum EMenuOption
{
	EMenuOption_StartGame,
	EMenuOption_Dummy,
};

struct MenuOption
{
	EMenuOption mOption;
	std::string mDisplayText;
};

static const MenuOption kOptions[] =
{
	{ EMenuOption_StartGame,	"Start Game" },
	{ EMenuOption_Dummy,		"Dummy Option" },
};

StartMenuState::StartMenuState()
	: mHighlightedMenuOption(0)
{
}

void StartMenuState::Reset()
{
	*this = StartMenuState();
}

void StartMenuState::Init()
{
}

EGameState StartMenuState::Update(float inFrameTime, const InputBuffer& inInput)
{
	EGameState outState = EGameState_StartMenu;
	
	if (inInput.IsPressed('w'))
	{
		mHighlightedMenuOption += (gElemCount(kOptions) - 1);
		mHighlightedMenuOption %= gElemCount(kOptions);
	}
	else if (inInput.IsPressed('s'))
	{
		mHighlightedMenuOption++;
		mHighlightedMenuOption %= gElemCount(kOptions);
	}

	if (inInput.IsPressed(' '))
	{
		switch (mHighlightedMenuOption)
		{
			case 0: outState = EGameState_InGame; break;
			default: break;
		}
	}

	return outState;
}

std::string StartMenuState::GetRenderBuffer()
{
	std::string out;

	for (size_t i = 0; i < gElemCount(kOptions); ++i)
	{
		out += (i == mHighlightedMenuOption) ? 26 : ' ';
		out += " ";
		out += kOptions[i].mDisplayText;
		out += "\n";
	}

	return out;
}
