#include "Shield.h"

#include <Containers/ContainerMacros.h>
#include <GameEntities/Player/PlayerEnums.h>

#include <EntityComponent/Components/AnimationComponent.h>
#include <EntityComponent/Components/CollisionMesh.h>
#include <EntityComponent/Components/CollisionComponent.h>
#include <EntityComponent/Components/PlayerComponent.h>
#include <EntityComponent/Components/PositionComponent.h>

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
	, mHeldUp(false)
{
}
	
void Shield::OnStartUsing(Entity inPlayer)
{
	using namespace Player;

	if (!mHeldUp)
	{
		inPlayer.GetComponent<AnimationComponent>()->SetAnimations( gCArrayToVector(kAnimations, gElemCount(kAnimations)) );
	
		UpdateUsing(inPlayer, 0.0f);

		mHeldUp = true;
	}
	else
	{
		// Switch to Idle - make no modifications.
		mHeldUp = false;
	}
}

bool Shield::UpdateUsing(Entity inPlayer, float /*inFrameTime*/)
{
	if (mHeldUp)
	{
		auto facingDirection = inPlayer.GetComponent<PlayerComponent>()->GetFacingDirection();
		inPlayer.GetComponent<CollisionComponent>()->SetCollisionMesh( kShieldCollisionMeshes[facingDirection] );
	}

	// We're finished if we're idle.
	return !mHeldUp;
}

void Shield::OnStoppedUsing(Entity /*inPlayer*/)
{
	mHeldUp = false;
}
