#pragma once

#include "StateBase.h"

#include "InGameScreenStates/InGamePlayScreen.h"
#include "InGameScreenStates/InventoryScreen.h"

class InputBuffer;

class InGameState : public StateBase
{
public:
	InGameState();

	virtual void		Reset();
	virtual void		Init();
	virtual EGameState	Update(float inFrameTime, const InputBuffer& inInput);
	virtual std::string GetRenderBuffer();

private:
	enum EGameScreen
	{
		EGameScreen_PlayScreen,
		EGameScreen_InventoryScreen,
	};

	InGamePlayScreen	mPlayScreen;
	InventoryScreen		mInventoryScreen;

	EGameScreen			mCurrentScreen;
};
