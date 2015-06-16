#include "Shield.h"

#include <Containers/ContainerMacros.h>
#include <GameEntities/Player/PlayerEnums.h>

#include <EntityComponent/Components/CollisionMesh.h>
#include <EntityComponent/Components/CollisionComponent.h>
// TODO: If this modifies collision, then we ned to reset it after we stop using this item...

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

static const AsciiMesh kShieldRenderMeshes[Player::EFacingDirection_Count] =
{
	AsciiMesh( kShieldFaceUp,		1, 2, IVec2( 0, -1) ),
	AsciiMesh( kShieldFaceDown,		1, 2, IVec2( 0,  0) ),
	AsciiMesh( kShieldFaceLeft,		2, 1, IVec2(-1,  0) ),
	AsciiMesh( kShieldFaceRight,	2, 1, IVec2( 0,  0) ),
};

//--------------------------------------------------------------------

static const Animation kAnimations[] =
{
	Animation( &kShieldRenderMeshes[0], 1, 0.0f, Animation::EPlaybackStyle_Once ),
	Animation( &kShieldRenderMeshes[1], 1, 0.0f, Animation::EPlaybackStyle_Once ),
	Animation( &kShieldRenderMeshes[2], 1, 0.0f, Animation::EPlaybackStyle_Once ),
	Animation( &kShieldRenderMeshes[3], 1, 0.0f, Animation::EPlaybackStyle_Once ),
};

static const char		kShieldIcon[]	= {	'[',254,']' };
static const ItemData	kShieldData		= { "Shield", AsciiMesh(kShieldIcon, gElemCount(kShieldIcon), 1) };

//--------------------------------------------------------------------

static const bool kShieldCollisionData[] = { true, true };

static const CollisionMesh kShieldCollisionMeshes[Player::EFacingDirection_Count] =
{
	CollisionMesh( kShieldCollisionData,	1, 2, IVec2( 0, -1) ),
	CollisionMesh( kShieldCollisionData,	1, 2, IVec2( 0,  0) ),
	CollisionMesh( kShieldCollisionData,	2, 1, IVec2(-1,  0) ),
	CollisionMesh( kShieldCollisionData,	2, 1, IVec2( 0,  0) ),
};

//--------------------------------------------------------------------

Shield::Shield()
	: ItemBase(kShieldData)
{
}
	
void Shield::Use(Entity inPlayer, bool inStartedUsingThisFrame)
{
}

std::vector<Animation> Shield::GetAnimations() const
{
	return gCArrayToVector(kAnimations, gElemCount(kAnimations));
}
