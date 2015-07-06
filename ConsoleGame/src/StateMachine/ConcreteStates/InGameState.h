#pragma once

#include <StateMachine/StateBase.h>

#include <Core/Messaging/MessageBroadcaster.h>
#include <EntityComponentSystem/World/World.h>
#include <GameSystems/CameraSystem.h>
#include <Dungeon/DungeonMap.h>
#include <UI/HUD.h>

class InputBuffer;

class InGameState : public StateBase
{
public:
	InGameState(MessageBroadcaster* inStateMachineMsgBroadcaster, GameData* inGameData);
	virtual ~InGameState() { }

	virtual void		Update(float inFrameTime, const InputBuffer& inInput);
	virtual std::string	GetRenderBuffer() const;

private:
	World				mWorld;
	DungeonMap			mDungeonMap;
	CameraSystem		mCameraSystem;
	HUD					mHUD;
	MessageBroadcaster	mMessageBroadcaster;
	Entity				mPlayer;
};
