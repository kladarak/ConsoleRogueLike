#pragma once

#include "PlayerEnums.h"
#include <Animations/Animation.h>

namespace Player
{
	
//--------------------------------------------------------------------
#define gElemCount( arr ) (sizeof(arr)/sizeof(arr[0]))

static const char kIdlePose = 'O';

static const AsciiMesh kIdleMeshes[EFacingDirection_Count] =
{
	AsciiMesh( &kIdlePose, 1, 1 ),
	AsciiMesh( &kIdlePose, 1, 1 ),
	AsciiMesh( &kIdlePose, 1, 1 ),
	AsciiMesh( &kIdlePose, 1, 1 ),
};

//--------------------------------------------------------------------

static const float kSwordKeyFrameDuration = 0.05f;

namespace SwordFaceUp
{
	static const char kSwordFaceUp0[]	= {	'_',
											'O' };
	static const char kSwordFaceUp1[]	= {	'\\',
											'O' };
	static const char kSwordFaceUp2[]	= {	' ', '|',
											'O', ' ' };

	static const AsciiMesh kSwordFaceUpKeyFrames[] =
	{
		AsciiMesh( kSwordFaceUp0,	1, 2, IVec2( 0, -1) ),
		AsciiMesh( kSwordFaceUp1,	1, 2, IVec2( 0, -1) ),
		AsciiMesh( kSwordFaceUp2,	2, 2, IVec2( 0, -1) ),
	};

	static const Animation kAnimation(	kSwordFaceUpKeyFrames, 
										gElemCount(kSwordFaceUpKeyFrames), 
										kSwordKeyFrameDuration, 
										Animation::EPlaybackStyle_Once );
}

//--------------------------------------------------------------------

namespace SwordFaceDown
{
	static const char kSwordFaceDown0[]	= {	'O',
											'-' };
	static const char kSwordFaceDown1[]	= {	'O',
											'\\' };
	static const char kSwordFaceDown2[]	= {	' ', 'O',
											'|', ' ' };

	static const AsciiMesh kSwordFaceDownKeyFrames[] =
	{
		AsciiMesh( kSwordFaceDown0,	1, 2, IVec2( 0, 0) ),
		AsciiMesh( kSwordFaceDown1,	1, 2, IVec2( 0, 0) ),
		AsciiMesh( kSwordFaceDown2,	2, 2, IVec2(-1, 0) ),
	};

	static const Animation kAnimation(	kSwordFaceDownKeyFrames, 
										gElemCount(kSwordFaceDownKeyFrames), 
										kSwordKeyFrameDuration, 
										Animation::EPlaybackStyle_Once );
}

//--------------------------------------------------------------------

namespace SwordFaceLeft
{
	static const char kSwordFaceLeft0[]	= { '|','O' };
	static const char kSwordFaceLeft1[]	= { '/','O' };
	static const char kSwordFaceLeft2[]	= { 238,'O' }; // should be "overscore" / "macron"

	static const AsciiMesh kSwordFaceLeftKeyFrames[] =
	{
		AsciiMesh( kSwordFaceLeft0,	2, 1, IVec2(-1, 0) ),
		AsciiMesh( kSwordFaceLeft1,	2, 1, IVec2(-1, 0) ),
		AsciiMesh( kSwordFaceLeft2,	2, 1, IVec2(-1, 0) ),
	};

	static const Animation kAnimation(	kSwordFaceLeftKeyFrames, 
										gElemCount(kSwordFaceLeftKeyFrames), 
										kSwordKeyFrameDuration, 
										Animation::EPlaybackStyle_Once );
}

//--------------------------------------------------------------------

namespace SwordFaceRight
{
	static const char kSwordFaceRight0[] = { 'O','|' };
	static const char kSwordFaceRight1[] = { 'O','/' };
	static const char kSwordFaceRight2[] = { 'O','_' };

	static const AsciiMesh kSwordFaceRightKeyFrames[] =
	{
		AsciiMesh( kSwordFaceRight0, 2, 1, IVec2(0, 0) ),
		AsciiMesh( kSwordFaceRight1, 2, 1, IVec2(0, 0) ),
		AsciiMesh( kSwordFaceRight2, 2, 1, IVec2(0, 0) ),
	};

	static const Animation kAnimation(	kSwordFaceRightKeyFrames, 
										gElemCount(kSwordFaceRightKeyFrames), 
										kSwordKeyFrameDuration, 
										Animation::EPlaybackStyle_Once );
}

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
	
	SwordFaceUp::kAnimation,
	SwordFaceDown::kAnimation,
	SwordFaceLeft::kAnimation,
	SwordFaceRight::kAnimation,

	Animation( &kShieldMeshes[0], 1, 0.0f, Animation::EPlaybackStyle_Once ),
	Animation( &kShieldMeshes[1], 1, 0.0f, Animation::EPlaybackStyle_Once ),
	Animation( &kShieldMeshes[2], 1, 0.0f, Animation::EPlaybackStyle_Once ),
	Animation( &kShieldMeshes[3], 1, 0.0f, Animation::EPlaybackStyle_Once ),
};

}
