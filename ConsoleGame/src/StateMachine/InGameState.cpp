#include "InGameState.h"

#include <Input/InputBuffer.h>

InGameState::InGameState()
	: mCurrentScreen(EGameScreen_PlayScreen)
{
}

void InGameState::Reset()
{
	*this = InGameState();
}

void InGameState::Init()
{
	mPlayScreen.Init();
	mInventoryScreen.Init( mPlayScreen.GetPlayer() );

	mCurrentScreen = EGameScreen_PlayScreen;
}

EGameState InGameState::Update(float inFrameTime, const InputBuffer& inInput)
{
	if (inInput.IsPressed('\t'))
	{
		switch (mCurrentScreen)
		{	
			case EGameScreen_PlayScreen:		mCurrentScreen = EGameScreen_InventoryScreen;	break;
			case EGameScreen_InventoryScreen:	mCurrentScreen = EGameScreen_PlayScreen;		break;
		}
	}

	bool running = true;

	switch (mCurrentScreen)
	{	
		case EGameScreen_PlayScreen:		running = mPlayScreen.Update(inFrameTime, inInput);	break;
		case EGameScreen_InventoryScreen:	mInventoryScreen.Update(inFrameTime, inInput);		break;
	}

	return running ? EGameState_InGame : EGameState_StartMenu;
}

std::string InGameState::GetRenderBuffer()
{
	switch (mCurrentScreen)
	{	
		case EGameScreen_PlayScreen:		return mPlayScreen.GetRenderBuffer();
		case EGameScreen_InventoryScreen:	return mInventoryScreen.GetRenderBuffer();
		default:							return "";
	}
}
