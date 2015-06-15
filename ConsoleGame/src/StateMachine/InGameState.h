#pragma once

#include "StateBase.h"

#include <Core/Messaging/MessageBroadcaster.h>

#include <EntityComponentSystem/World/World.h>

#include <GameSystems/CameraSystem.h>
#include <Dungeon/DungeonMap.h>
#include <UI/HUD.h>

class InputBuffer;

class InGameState : public StateBase
{
public:
	virtual void		Reset();
	virtual void		Init();
	virtual EGameState	Update(float inFrameTime, const InputBuffer& inInput);
	virtual std::string GetRenderBuffer();

private:
	World				mWorld;
	DungeonMap			mDungeonMap;
	CameraSystem		mCameraSystem;
	HUD					mHUD;
	MessageBroadcaster	mMessageBroadcaster;

};
