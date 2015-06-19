#pragma once

#include <string>

class YouAreDeadDisplay
{
public:
	YouAreDeadDisplay();

	void		ResetAnimation();
	void		UpdateAnimation(float inFrameTime);
	bool		IsAnimationFinished() const;

	std::string GetRenderBuffer() const;

private:
	float		mElapsedTime;
};
