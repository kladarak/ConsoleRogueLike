#include "DoorKey.h"

#include <Containers/ContainerMacros.h>

#include <GameEntities/Player/PlayerConstants.h>

#include <EntityComponent/Components/AnimationComponent.h>
#include <EntityComponent/Components/CollisionComponent.h>
#include <EntityComponent/Components/OrientationComponent.h>
#include <EntityComponent/Components/PositionComponent.h>

#include <Messages/Messages.h>

#include "ItemData.h"

//--------------------------------------------------------------------

static const Fragment kDoorKeyFaceUp[] =
{
	Fragment('|', ETextYellow),
	PlayerConstants::kSprite,
};

static const Fragment kDoorKeyFaceDown[] =
{
	PlayerConstants::kSprite,
	Fragment('|', ETextYellow),
};

static const Fragment kDoorKeyFaceLeft[]	= { Fragment('-', ETextYellow), PlayerConstants::kSprite  };
static const Fragment kDoorKeyFaceRight[]	= { PlayerConstants::kSprite, Fragment('-', ETextYellow) };

static const AsciiMesh kDoorKeyRenderMeshes[EOrientation_Count] =
{
	AsciiMesh( kDoorKeyFaceUp,		1, 2, IVec2( 0, -1) ),
	AsciiMesh( kDoorKeyFaceDown,	1, 2, IVec2( 0,  0) ),
	AsciiMesh( kDoorKeyFaceLeft,	2, 1, IVec2(-1,  0) ),
	AsciiMesh( kDoorKeyFaceRight,	2, 1, IVec2( 0,  0) ),
};

//--------------------------------------------------------------------

static std::vector<Animation> sGenerateAnimations(EColour inKeyColour)
{
	std::vector<Animation> animations;

	for (int i = 0; i < 4; ++i)
	{
		AsciiMesh newMesh = gReColourAsciiMesh(kDoorKeyRenderMeshes[i], inKeyColour);
		animations.push_back( Animation(&newMesh, 1, 0.0f, Animation::EPlaybackStyle_Once) );
	}

	return animations;
}

//--------------------------------------------------------------------

const std::string DoorKey::kName = "Key";

static const CollisionMesh	kDoorKeyCollisionMesh( 0, 0 );

static const Fragment		kDoorKeyIcon[]	= { Fragment('0', ETextYellow), Fragment('-', ETextYellow), Fragment('m', ETextYellow) };
static const AsciiMesh		kDoorKeyIconMesh( AsciiMesh(kDoorKeyIcon, gElemCount(kDoorKeyIcon), 1) );
static const ItemData		kDoorKeyData( DoorKey::kName, kDoorKeyIconMesh, ERequiresNoAmmo );
static const float			kAnimTime	= 0.15f;

static const ItemData sGenerateDoorKeyData(EColour inKeyColour)
{
	return ItemData(DoorKey::kName, gReColourAsciiMesh(kDoorKeyIconMesh, inKeyColour), ERequiresNoAmmo);
}

using namespace Player;

DoorKey::DoorKey(EColour inKeyColour) 
	: ItemBase		(sGenerateDoorKeyData(inKeyColour))
	, mBehaviour	(inKeyColour)
{
}

void DoorKeyPlayerBehaviour::OnStart(Entity inPlayer)
{
	inPlayer.GetComponent<AnimationComponent>()->SetAnimations( sGenerateAnimations(mKeyColour) );
	inPlayer.GetComponent<CollisionComponent>()->SetDefaultCollisionMesh( kDoorKeyCollisionMesh );

	mAnimTimeElapsed = 0.0f;
	
	auto	playerPos	= inPlayer.GetComponent<PositionComponent>()->GetPosition();
	auto	orientation	= inPlayer.GetComponent<OrientationComponent>()->GetOrientation();
	IVec2	unlockDir	= gGetOrientationVector(orientation);

	IVec2			unlockPos = playerPos + unlockDir;
	UnlockDoorMsg	unlockDoorkMsg(unlockPos, mKeyColour);
	MessageHelpers::BroadcastMessageToEntitiesAtPosition(*inPlayer.GetWorld(), inPlayer, unlockPos, unlockDoorkMsg);
}

void DoorKeyPlayerBehaviour::OnRestart(Entity inPlayer)
{
	OnStart(inPlayer);
}

void DoorKeyPlayerBehaviour::Update(Entity /*inPlayer*/, float inFrameTime)
{
	mAnimTimeElapsed += inFrameTime;
}

void DoorKeyPlayerBehaviour::OnFinish(Entity /*inPlayer*/)
{
}

bool DoorKeyPlayerBehaviour::IsFinished() const
{ 
	return mAnimTimeElapsed > kAnimTime;
}
