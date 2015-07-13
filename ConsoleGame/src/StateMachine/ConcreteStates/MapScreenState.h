#pragma once

#include <StateMachine/StateBase.h>

#include <Maths/IVec2.h>
#include <Renderer/RenderTarget.h>

class MapScreenState : public StateBase
{
public:
	MapScreenState(MessageBroadcaster* inStateMachineMsgBroadcaster, GameData* inGameData);

	virtual void		Update(float inFrameTime, const InputBuffer& inInput);
	virtual std::string GetRenderBuffer() const;

private:
	RenderTarget		mBasicMap;
	IVec2				mViewOffset;
	float				mPlayerIconFlashTime;
};
