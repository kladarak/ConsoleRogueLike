#include "Shield.h"

#include <Containers/ContainerMacros.h>
#include <GameEntities/Player/PlayerEnums.h>

#include <EntityComponent/Components/AnimationComponent.h>
#include <EntityComponent/Components/CollisionComponent.h>
#include <EntityComponent/Components/MonsterComponent.h>
#include <EntityComponent/Components/OrientationComponent.h>
#include <EntityComponent/Components/PlayerComponent.h>
#include <EntityComponent/Components/PositionComponent.h>

#include <EntityComponent/Systems/CollisionSystem.h>

#include <Messages/Messages.h>

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

static const AsciiMesh kShieldRenderMeshes[EOrientation_Count] =
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

static const char		kShieldIcon[]	= {	'[', gCastUCharToChar(254) ,']' };
static const ItemData	kShieldData("Shield", AsciiMesh(kShieldIcon, gElemCount(kShieldIcon), 1), ERequiresNoAmmo);

//--------------------------------------------------------------------

static const bool kShieldCollisionData[] = { true, true };

static const CollisionMesh kShieldCollisionMeshes[EOrientation_Count] =
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

//--------------------------------------------------------------------

void ShieldPlayerBehaviour::OnStart(Entity inPlayer)
{
	using namespace Player;

	mHeldUp = true;
	inPlayer.GetComponent<AnimationComponent>()->SetAnimations( gCArrayToVector(kAnimations, gElemCount(kAnimations)) );
	inPlayer.GetComponent<CollisionComponent>()->SetCollisionMeshes( kShieldCollisionMeshes );
}

void ShieldPlayerBehaviour::OnRestart(Entity /*inPlayer*/)
{
	mHeldUp = false;
}

void ShieldPlayerBehaviour::Update(Entity inPlayer, float /*inFrameTime*/)
{
	if (mHeldUp)
	{
		auto	posComp		= inPlayer.GetComponent<PositionComponent>();
		auto&	position	= posComp->GetPosition();

		if (CollisionSystem::CollidesWithAnyEntity(*inPlayer.GetWorld(), inPlayer))
		{
			auto orientation	= inPlayer.GetComponent<OrientationComponent>()->GetOrientation();
			auto pushBackVector = -gGetOrientationVector(orientation);

			auto pushedBackPos = position + pushBackVector;

			if (!CollisionSystem::CollidesWithAnyEntity(*inPlayer.GetWorld(), inPlayer, pushedBackPos))
			{
				posComp->SetPosition(pushedBackPos);
			}
		}
	}
}

void ShieldPlayerBehaviour::OnFinish(Entity /*inPlayer*/)
{
	mHeldUp = false;
}

bool ShieldPlayerBehaviour::IsFinished() const
{
	return !mHeldUp;
}

bool ShieldPlayerBehaviour::CanMoveToPosition(Entity inPlayer, const IVec2& inPosition) const
{
	auto& playerCollisionMesh = CollisionSystem::GetActiveCollisionMesh(inPlayer);
	auto entitiesCollidedWith = CollisionSystem::GetListofCollidablesCollidedWith(*inPlayer.GetWorld(), playerCollisionMesh, inPosition);
	
	for (auto& collidable : entitiesCollidedWith)
	{
		if (collidable != inPlayer)
		{
			return false;
		}
	}

	return true;
}

bool ShieldPlayerBehaviour::OnAttacked(Entity inPlayer, const AttackMsg& inAttackMsg)
{
	// If it hits the player, then it injures it. Otherwise the shield was hit, and so it doesn't injure the player.
	auto position = inPlayer.GetComponent<PositionComponent>()->GetPosition();
	return (position == inAttackMsg.mAttackPosition);
}
