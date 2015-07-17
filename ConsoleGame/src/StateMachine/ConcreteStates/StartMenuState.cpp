#include "StartMenuState.h"

#include <Containers/ContainerMacros.h>
#include <Input/InputBuffer.h>
#include <Messages/Messages.h>
#include <Renderer/RenderTargetWriter.h>


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
	//{ EMenuOption_Dummy,		"Dummy Option" },
};

StartMenuState::StartMenuState(MessageBroadcaster* inStateMachineMsgBroadcaster, GameData* inGameData)
	: StateBase(inStateMachineMsgBroadcaster, inGameData)
	, mHighlightedMenuOption(0)
{
}

void StartMenuState::Update(float /*inFrameTime*/, const InputBuffer& inInput)
{
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
			case EMenuOption_StartGame: RequestGoToState(EGameState_StartLevelIntro); break;
			default: break;
		}
	}
}

RenderTarget StartMenuState::GetRenderTarget() const
{
	RenderTargetWriter writer(20, gElemCount(kOptions));

	for (size_t i = 0; i < gElemCount(kOptions); ++i)
	{
		std::string str;
		str += (i == mHighlightedMenuOption) ? 26 : ' ';
		str += " ";
		str += kOptions[i].mDisplayText;

		writer.Write(str, 0, i);
	}

	return writer.GetRenderTarget();
}
