#pragma once

#include "PlayerEnums.h"
#include <Renderer/AsciiMesh.h>

namespace Player
{
	
static const AsciiMesh kIdleMeshes[EFacingDirection_Count] =
{
	AsciiMesh( { { IVec2(0, 0), 'O' } } ),
	AsciiMesh( { { IVec2(0, 0), 'O' } } ),
	AsciiMesh( { { IVec2(0, 0), 'O' } } ),
	AsciiMesh( { { IVec2(0, 0), 'O' } } ),
};

static const AsciiMesh kSwordMeshes[EFacingDirection_Count] =
{
	AsciiMesh( { { IVec2(0, 0), 'O' }, { IVec2( 0, -1), '\\' } } ),
	AsciiMesh( { { IVec2(0, 0), 'O' }, { IVec2( 0,  1), '\\' } } ),
	AsciiMesh( { { IVec2(0, 0), 'O' }, { IVec2(-1,  0), '/' } } ),
	AsciiMesh( { { IVec2(0, 0), 'O' }, { IVec2( 1,  0), '/' } } ),
};

static const AsciiMesh kShieldMeshes[EFacingDirection_Count] =
{
	AsciiMesh( { { IVec2(0, 0), 'O' }, { IVec2( 0, -1), '-' } } ),
	AsciiMesh( { { IVec2(0, 0), 'O' }, { IVec2( 0,  1), '-' } } ),
	AsciiMesh( { { IVec2(0, 0), 'O' }, { IVec2(-1,  0), '|' } } ),
	AsciiMesh( { { IVec2(0, 0), 'O' }, { IVec2( 1,  0), '|' } } ),
};

}
