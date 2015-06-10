#pragma once

#include "PlayerEnums.h"
#include <Renderer/AsciiMesh.h>

namespace Player
{
	
static const AsciiMesh kIdleMeshes[EFacingDirection_Count] =
{
	AsciiMesh( { { 0, 0, 'O' } } ),
	AsciiMesh( { { 0, 0, 'O' } } ),
	AsciiMesh( { { 0, 0, 'O' } } ),
	AsciiMesh( { { 0, 0, 'O' } } ),
};

static const AsciiMesh kSwordMeshes[EFacingDirection_Count] =
{
	AsciiMesh( { { 0, 0, 'O' }, {  0, -1, '\\' } } ),
	AsciiMesh( { { 0, 0, 'O' }, {  0,  1, '\\' } } ),
	AsciiMesh( { { 0, 0, 'O' }, { -1,  0, '/' } } ),
	AsciiMesh( { { 0, 0, 'O' }, {  1,  0, '/' } } ),
};

static const AsciiMesh kShieldMeshes[EFacingDirection_Count] =
{
	AsciiMesh( { { 0, 0, 'O' }, {  0, -1, '-' } } ),
	AsciiMesh( { { 0, 0, 'O' }, {  0,  1, '-' } } ),
	AsciiMesh( { { 0, 0, 'O' }, { -1,  0, '|' } } ),
	AsciiMesh( { { 0, 0, 'O' }, {  1,  0, '|' } } ),
};

}
