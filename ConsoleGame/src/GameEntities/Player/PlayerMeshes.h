#pragma once

#include <Animations/Animation.h>
#include <Containers/ContainerMacros.h>

#include "PlayerEnums.h"

namespace Player
{
	
static const AsciiMesh kIdleMesh( 'O' );

static const Animation kAnimations[] =
{
	Animation( &kIdleMesh, 1, 0.0f, Animation::EPlaybackStyle_Once ),
	Animation( &kIdleMesh, 1, 0.0f, Animation::EPlaybackStyle_Once ),
	Animation( &kIdleMesh, 1, 0.0f, Animation::EPlaybackStyle_Once ),
	Animation( &kIdleMesh, 1, 0.0f, Animation::EPlaybackStyle_Once ),
};

static inline std::vector<Animation> GetAnimations()
{
	std::vector<Animation> animations;

	for (size_t i = 0; i < gElemCount(kAnimations); ++i)
	{
		animations.push_back(kAnimations[i]);
	}

	return animations;
}

}
