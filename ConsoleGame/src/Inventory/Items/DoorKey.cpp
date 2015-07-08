#include "DoorKey.h"

#include <Containers/ContainerMacros.h>

#include <EntityComponent/Components/AnimationComponent.h>
#include <EntityComponent/Components/CollisionComponent.h>
#include <EntityComponent/Components/OrientationComponent.h>
#include <EntityComponent/Components/PositionComponent.h>

#include <Messages/Messages.h>

#include "ItemData.h"

//--------------------------------------------------------------------

static const char kDoorKeyFaceUp[] =
{
	'|',
	'O',
};

static const char kDoorKeyFaceDown[] =
{
	'O',
	'|',
};

static const char kDoorKeyFaceLeft[]	= { '-','O' };
static const char kDoorKeyFaceRight[]	= { 'O','-' };

static const AsciiMesh kDoorKeyRenderMeshes[EOrientation_Count] =
{
	AsciiMesh( kDoorKeyFaceUp,		1, 2, IVec2( 0, -1) ),
	AsciiMesh( kDoorKeyFaceDown,	1, 2, IVec2( 0,  0) ),
	AsciiMesh( kDoorKeyFaceLeft,	2, 1, IVec2(-1,  0) ),
	AsciiMesh( kDoorKeyFaceRight,	2, 1, IVec2( 0,  0) ),
};

//--------------------------------------------------------------------

static const Animation kAnimations[] =
{
	Animation( &kDoorKeyRenderMeshes[0], 1, 0.0f, Animation::EPlaybackStyle_Once ),
	Animation( &kDoorKeyRenderMeshes[1], 1, 0.0f, Animation::EPlaybackStyle_Once ),
	Animation( &kDoorKeyRenderMeshes[2], 1, 0.0f, Animation::EPlaybackStyle_Once ),
	Animation( &kDoorKeyRenderMeshes[3], 1, 0.0f, Animation::EPlaybackStyle_Once ),
};

//--------------------------------------------------------------------

const std::string DoorKey::kName = "Key";

static const CollisionMesh	kDoorKeyCollisionMesh( 0, 0 );

static const char			kDoorKeyIcon[]	= { '0', '-', 'm' };
static const ItemData		kDoorKeyData	= { DoorKey::kName, AsciiMesh(kDoorKeyIcon, gElemCount(kDoorKeyIcon), 1) };
static const float			kAnimTime	= 0.15f;

using namespace Player;

DoorKey::DoorKey() 
	: ItemBase(kDoorKeyData)
{
}

void DoorKeyPlayerBehaviour::OnStart(Entity inPlayer)
{
	inPlayer.GetComponent<AnimationComponent>()->SetAnimations( gCArrayToVector(kAnimations, gElemCount(kAnimations)) );
	inPlayer.GetComponent<CollisionComponent>()->SetDefaultCollisionMesh( kDoorKeyCollisionMesh );

	mAnimTimeElapsed = 0.0f;
	
	auto	playerPos	= inPlayer.GetComponent<PositionComponent>()->GetPosition();
	auto	orientation	= inPlayer.GetComponent<OrientationComponent>()->GetOrientation();
	IVec2	unlockDir	= gGetOrientationVector(orientation);

	IVec2			unlockPos = playerPos + unlockDir;
	UnlockDoorMsg	unlockDoorkMsg(unlockPos);
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
