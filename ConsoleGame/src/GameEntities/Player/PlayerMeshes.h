#pragma once

#include <Animations/Animation.h>
#include <Containers/ContainerMacros.h>

#include "PlayerEnums.h"

namespace Player
{
	
static const AsciiMesh		kIdleRenderMesh( 'O' );
static const CollisionMesh	kIdleCollisionMesh( 0, 0 );

static const Animation kIdleAnimations[] =
{
	Animation( &kIdleRenderMesh, 1, 0.0f, Animation::EPlaybackStyle_Once ),
	Animation( &kIdleRenderMesh, 1, 0.0f, Animation::EPlaybackStyle_Once ),
	Animation( &kIdleRenderMesh, 1, 0.0f, Animation::EPlaybackStyle_Once ),
	Animation( &kIdleRenderMesh, 1, 0.0f, Animation::EPlaybackStyle_Once ),
};

static inline std::vector<Animation> GetIdleAnimations()
{
	return gCArrayToVector(kIdleAnimations, gElemCount(kIdleAnimations));
}

}
