#include "YouAreDeadDisplay.h"

#include <Maths/Maths.h>


namespace
{
	const float			kAnimationDuration	= 3.0f;
	const std::string	kMessage			= "You Are Dead.";
}

YouAreDeadDisplay::YouAreDeadDisplay()
	: mElapsedTime(0.0f)
{
}

void YouAreDeadDisplay::ResetAnimation()
{
	mElapsedTime = 0.0f;
}

void YouAreDeadDisplay::UpdateAnimation(float inFrameTime)
{
	mElapsedTime += inFrameTime;
}

bool YouAreDeadDisplay::IsAnimationFinished() const
{
	return mElapsedTime >= kAnimationDuration;
}

RenderTarget YouAreDeadDisplay::GetRenderTarget() const
{
	static const float kKeyFrame0 = 0.0f;
	static const float kKeyFrame1 = 0.5f;
	static const float kKeyFrame2 = 1.5f;
	static const float kKeyFrame3 = 2.0f;

	RenderTarget renderTarget(kMessage.size(), 1);

	if ( gIsBetween(mElapsedTime, kKeyFrame0, kKeyFrame1) )
	{
		// Do nothing
	}
	else if ( gIsBetween(mElapsedTime, kKeyFrame1, kKeyFrame2) )
	{
		float	t				= gFraction(mElapsedTime, kKeyFrame1, kKeyFrame2);
		size_t	lettersToShow	= (size_t) gLerp2D(t, 0.0f, (float) kMessage.size());
		renderTarget.Write(kMessage.substr(0, lettersToShow), ETextWhite, 0, 0);
	}
	else// if ( mElapsedTime >= kKeyFrame2 )
	{
		renderTarget.Write(kMessage, ETextWhite, 0, 0);
	}

	return renderTarget;
}
