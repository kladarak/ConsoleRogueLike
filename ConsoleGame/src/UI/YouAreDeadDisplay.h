#pragma once

#include <string>
#include <Renderer/RenderTarget.h>

class YouAreDeadDisplay
{
public:
	YouAreDeadDisplay();

	void			ResetAnimation();
	void			UpdateAnimation(float inFrameTime);
	bool			IsAnimationFinished() const;

	RenderTarget	GetRenderTarget() const;

private:
	float			mElapsedTime;
};
