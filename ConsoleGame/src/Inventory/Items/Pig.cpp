#include "Pig.h"

#include <Containers/ContainerMacros.h>

#include <EntityComponent/Components/AnimationComponent.h>
#include <EntityComponent/Components/CollisionComponent.h>
#include <EntityComponent/Components/MessageReceiverComponent.h>
#include <EntityComponent/Components/OrientationComponent.h>
#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/RenderableComponent.h>

#include <GameEntities/Player/PlayerConstants.h>

#include <Messages/Messages.h>

namespace
{

	namespace FaceLeft
	{
		//    ___&
		//  e'^_ )
		//    " "
		const Fragment kPigFrags[] =
		{
			Fragment(' ', EInvisible),		Fragment(' ', EInvisible),		Fragment('_', ETextMagenta),  PlayerConstants::kSprite,		Fragment('_', ETextMagenta),  Fragment('&', ETextMagenta),
			Fragment('e', ETextMagenta),	Fragment('\'', ETextMagenta),	Fragment('^', ETextMagenta),  Fragment('_', ETextMagenta),	Fragment(' ', ETextMagenta),  Fragment(')', ETextMagenta),
			Fragment(' ', EInvisible),		Fragment(' ', EInvisible),		Fragment('\"', ETextMagenta), Fragment(' ', EInvisible),	Fragment('\"', ETextMagenta), Fragment(' ', EInvisible),
		};

		const bool kPigCollisionMeshFrags[] =
		{
			false,	false,	true,	true,	true,	true,
			true,	true,	true,	true,	true,	true,
			false,	false,	true,	false,	true,	false,
		};

		const AsciiMesh		kPigRenderMesh(kPigFrags, 6, 3, IVec2(-3, 0));
		const CollisionMesh kPigCollisionMesh(kPigCollisionMeshFrags, 6, 3);

		const Animation kPigIdleAnimation(&kPigRenderMesh, 1, 0.0f, Animation::EPlaybackStyle_Loop);

		// One for each EOrientation.
		const Animation kAnimationSet[] =
		{
			kPigIdleAnimation,
			kPigIdleAnimation,
			kPigIdleAnimation,
			kPigIdleAnimation,
		};
	}

	namespace FaceRight
	{
		//  &___
		//  ( _^'g
		//   " "
		const Fragment kPigFrags[] =
		{
			Fragment('&', ETextMagenta),	Fragment('_', ETextMagenta),	PlayerConstants::kSprite,		Fragment('_', ETextMagenta),	Fragment(' ', EInvisible),		Fragment(' ', EInvisible),
			Fragment('(', ETextMagenta),	Fragment(' ', ETextMagenta),	Fragment('_', ETextMagenta),	Fragment('^', ETextMagenta),	Fragment('\'', ETextMagenta),	Fragment('g', ETextMagenta),
			Fragment(' ', EInvisible),		Fragment('"', ETextMagenta),	Fragment(' ', EInvisible),		Fragment('"', ETextMagenta),	Fragment(' ', EInvisible),		Fragment(' ', EInvisible),
		};

		const bool kPigCollisionMeshFrags[] =
		{
			true,	true,	true,	true,	false,	false,
			true,	true,	true,	true,	true,	true,
			false,	true,	false,	true,	false,	false,
		};

		const AsciiMesh		kPigRenderMesh(kPigFrags, 6, 3, IVec2(-2, 0));
		const CollisionMesh kPigCollisionMesh(kPigCollisionMeshFrags, 6, 3);

		const Animation kPigIdleAnimation(&kPigRenderMesh, 1, 0.0f, Animation::EPlaybackStyle_Loop);

		// One for each EOrientation.
		const Animation kAnimationSet[] =
		{
			kPigIdleAnimation,
			kPigIdleAnimation,
			kPigIdleAnimation,
			kPigIdleAnimation,
		};
	}
}

static const Fragment kHUDIcon[] =
{
	Fragment('P', ETextMagenta),
	Fragment('I', ETextMagenta),
	Fragment('G', ETextMagenta),
}; 

static const ItemData	kPigItemData("Pig", AsciiMesh(kHUDIcon, gElemCount(kHUDIcon), 1), ERequiresNoAmmo);
static const float		kTimeBetweenMovement = 0.05f;


using namespace Player;

Pig::Pig() 
	:	ItemBase(kPigItemData)
{
}

void PigRiderPlayerBehaviour::OnStart(Entity inPlayer)
{
	mIsOnPig = !mIsOnPig;

	if (mIsOnPig)
	{
		inPlayer.GetComponent<AnimationComponent>()->SetAnimations( FaceLeft::kAnimationSet, 4 );
		inPlayer.GetComponent<CollisionComponent>()->SetDefaultCollisionMesh( FaceLeft::kPigCollisionMesh );
	}
}

void PigRiderPlayerBehaviour::OnRestart(Entity inPlayer)
{
	OnStart(inPlayer);
}

void PigRiderPlayerBehaviour::Update(Entity inPlayer, float inFrameTime)
{
	mTimeSinceMovement += inFrameTime;

	if (mTimeSinceMovement < kTimeBetweenMovement || !mIsOnPig)
	{
		return;
	}

	mTimeSinceMovement -= kTimeBetweenMovement;

	auto	posComp			= inPlayer.GetComponent<PositionComponent>();
	auto	position		= posComp->GetPosition();
	auto	direction		= (EOrientation) (rand() % 4);

	IVec2	stepDirection	= gGetOrientationVector(direction);
	IVec2	newPosition		= position + stepDirection;

	if (CanMoveToPosition(inPlayer, newPosition))
	{
		posComp->SetPosition(newPosition);
		position = newPosition;

		if (direction == EOrientation_FaceLeft)
		{
			inPlayer.GetComponent<AnimationComponent>()->SetAnimations( FaceLeft::kAnimationSet, 4 );
			inPlayer.GetComponent<CollisionComponent>()->SetDefaultCollisionMesh( FaceLeft::kPigCollisionMesh );
		}
		else if (direction == EOrientation_FaceRight)
		{
			inPlayer.GetComponent<AnimationComponent>()->SetAnimations( FaceRight::kAnimationSet, 4 );
			inPlayer.GetComponent<CollisionComponent>()->SetDefaultCollisionMesh( FaceRight::kPigCollisionMesh );
		}
	}

	inPlayer.GetComponent<CollisionComponent>()->GetDefaultCollisionMesh().ForEachCollidablePosition( [&] (int inX, int inY)
	{
		IVec2		attackPos = position + IVec2(inX, inY);
		AttackMsg	attackMsg(inPlayer, attackPos, IVec2(0, 0), AttackMsg::EEffect_None);
		MessageHelpers::BroadcastMessageToEntitiesAtPosition(*inPlayer.GetWorld(), inPlayer, attackPos, attackMsg);
	} );
}

void PigRiderPlayerBehaviour::OnFinish(Entity /*inPlayer*/)
{
	mIsOnPig = false;
}

bool PigRiderPlayerBehaviour::IsFinished() const
{ 
	return !mIsOnPig;
}
