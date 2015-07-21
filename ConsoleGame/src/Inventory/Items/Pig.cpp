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
	//    ___&
	//  e'^_ )
	//    " "
	const Fragment kPigFrags[] =
	{
		Fragment(' ', EInvisible),		Fragment(' ', EInvisible),		Fragment('_', ETextMagenta),  PlayerConstants::kSprite,		Fragment('_', ETextMagenta),  Fragment('&', ETextMagenta),
		Fragment('e', ETextMagenta),	Fragment('\'', ETextMagenta),	Fragment('^', ETextMagenta),  Fragment('_', ETextMagenta),	Fragment(' ', ETextMagenta),  Fragment(')', ETextMagenta),
		Fragment(' ', EInvisible),		Fragment(' ', EInvisible),		Fragment('\"', ETextMagenta), Fragment(' ', EInvisible),	Fragment('\"', ETextMagenta), Fragment(' ', EInvisible),
	};

	const AsciiMesh kPigMesh(kPigFrags, 6, 3, IVec2(-3, 0));

	const Animation kAnimations[] =
	{
		Animation(&kPigMesh, 1, 0.0f, Animation::EPlaybackStyle_Loop),
		Animation(&kPigMesh, 1, 0.0f, Animation::EPlaybackStyle_Loop),
		Animation(&kPigMesh, 1, 0.0f, Animation::EPlaybackStyle_Loop),
		Animation(&kPigMesh, 1, 0.0f, Animation::EPlaybackStyle_Loop),
	};

	const bool kPigCollisionMeshFrags[] =
	{
		false,	false,	true,	true,	true,	true,
		true,	true,	true,	true,	true,	true,
		false,	false,	true,	false,	true,	false,
	};

	const CollisionMesh kPigCollisionMesh(kPigCollisionMeshFrags, 6, 3);
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
		inPlayer.GetComponent<AnimationComponent>()->SetAnimations( kAnimations, 4 );
		inPlayer.GetComponent<CollisionComponent>()->SetDefaultCollisionMesh( kPigCollisionMesh );
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
	}

	kPigCollisionMesh.ForEachCollidablePosition( [&] (int inX, int inY)
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
