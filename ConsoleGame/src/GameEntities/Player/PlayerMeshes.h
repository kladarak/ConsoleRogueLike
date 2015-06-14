#pragma once

#include <Animations/Animation.h>
#include <Containers/ContainerMacros.h>

#include "PlayerEnums.h"

namespace Player
{
	
//--------------------------------------------------------------------

static const char kIdlePose = 'O';

static const AsciiMesh kIdleMeshes[EFacingDirection_Count] =
{
	AsciiMesh( &kIdlePose, 1, 1 ),
	AsciiMesh( &kIdlePose, 1, 1 ),
	AsciiMesh( &kIdlePose, 1, 1 ),
	AsciiMesh( &kIdlePose, 1, 1 ),
};

//--------------------------------------------------------------------

static const char kShieldFaceUp[] =
{
	'-',
	'O',
};

static const char kShieldFaceDown[] =
{
	'O',
	'-',
};

static const char kShieldFaceLeft[]	= { '|','O' };
static const char kShieldFaceRight[] = { 'O','|' };

static const AsciiMesh kShieldMeshes[EFacingDirection_Count] =
{
	AsciiMesh( kShieldFaceUp,		1, 2, IVec2( 0, -1) ),
	AsciiMesh( kShieldFaceDown,		1, 2, IVec2( 0,  0) ),
	AsciiMesh( kShieldFaceLeft,		2, 1, IVec2(-1,  0) ),
	AsciiMesh( kShieldFaceRight,	2, 1, IVec2( 0,  0) ),
};

//--------------------------------------------------------------------

static const Animation kAnimations[] =
{
	Animation( &kIdleMeshes[0], 1, 0.0f, Animation::EPlaybackStyle_Once ),
	Animation( &kIdleMeshes[1], 1, 0.0f, Animation::EPlaybackStyle_Once ),
	Animation( &kIdleMeshes[2], 1, 0.0f, Animation::EPlaybackStyle_Once ),
	Animation( &kIdleMeshes[3], 1, 0.0f, Animation::EPlaybackStyle_Once ),

	Animation( &kShieldMeshes[0], 1, 0.0f, Animation::EPlaybackStyle_Once ),
	Animation( &kShieldMeshes[1], 1, 0.0f, Animation::EPlaybackStyle_Once ),
	Animation( &kShieldMeshes[2], 1, 0.0f, Animation::EPlaybackStyle_Once ),
	Animation( &kShieldMeshes[3], 1, 0.0f, Animation::EPlaybackStyle_Once ),
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
