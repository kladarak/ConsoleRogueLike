#include "YouAreDeadDisplay.h"

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

std::string YouAreDeadDisplay::GetRenderBuffer() const
{
	static const float kKeyFrame0 = 0.0f;
	static const float kKeyFrame1 = 0.5f;
	static const float kKeyFrame2 = 1.5f;
	static const float kKeyFrame3 = 2.0f;

	std::string outBuffer;

	auto isTimeBetween	= [&] (float inMin, float inMax)			{ return (mElapsedTime >= inMin) && (mElapsedTime < inMax); };
	auto lerp			= [&] (float inT, float inMin, float inMax) { return inT * (inMax - inMin) + inMin;		};
	auto proportion		= [&] (float inT, float inMin, float inMax) { return (inT - inMin) / (inMax - inMin);	};

	if ( isTimeBetween(kKeyFrame0, kKeyFrame1) )
	{
		// Do nothing
	}
	else if ( isTimeBetween(kKeyFrame1, kKeyFrame2) )
	{
		float	t				= proportion(mElapsedTime, kKeyFrame1, kKeyFrame2);
		size_t	lettersToShow	= (size_t) lerp(t, 0.0f, (float) kMessage.size());
		outBuffer = kMessage.substr(0, lettersToShow);
	}
	else// if ( mElapsedTime >= kKeyFrame2 )
	{
		outBuffer = kMessage;
	}

	return outBuffer;
}
