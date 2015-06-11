#pragma once

#include "PlayerEnums.h"
#include <Renderer/AsciiMesh.h>

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

static const char kSwordFaceUp[] =
{
	'\\',
	'O',
};

static const char kSwordFaceDown[] =
{
	'O',
	'\\',
};

static const char kSwordFaceLeft[]	= { '/','O' };
static const char kSwordFaceRight[] = { 'O','/' };

static const AsciiMesh kSwordMeshes[EFacingDirection_Count] =
{
	AsciiMesh( kSwordFaceUp,	1, 2, IVec2( 0, -1) ),
	AsciiMesh( kSwordFaceDown,	1, 2, IVec2( 0,  0) ),
	AsciiMesh( kSwordFaceLeft,	2, 1, IVec2(-1,  0) ),
	AsciiMesh( kSwordFaceRight, 2, 1, IVec2( 0,  0) ),
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

}
