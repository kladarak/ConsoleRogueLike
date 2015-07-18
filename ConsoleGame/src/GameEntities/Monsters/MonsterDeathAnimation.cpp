#include "MonsterDeathAnimation.h"

#include <Containers/ContainerMacros.h>

namespace MonsterDeathAnimation
{
	static const AsciiMesh kDeathFrames[] =
	{
		gCastUCharToChar(178),
		gCastUCharToChar(177),
		gCastUCharToChar(176),
		gCastUCharToChar(250),
	};
	
	static const float		kDeathAnimationDuration = 1.5f;
	static const Animation	kAnimation(kDeathFrames, gElemCount(kDeathFrames), kDeathAnimationDuration/gElemCount(kDeathFrames), Animation::EPlaybackStyle_Once);

	float sGetDuration()
	{
		return kDeathAnimationDuration;
	}

	const Animation& sGetAnimation()
	{
		return kAnimation;
	}
}
