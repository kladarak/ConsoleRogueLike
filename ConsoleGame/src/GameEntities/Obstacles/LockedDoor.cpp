#include "LockedDoor.h"

#include <Core/Debug/Assert.h>

#include <EntityComponent/Components/AnimationComponent.h>
#include <EntityComponent/Components/CollisionComponent.h>
#include <EntityComponent/Components/MessageReceiverComponent.h>
#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/RenderableComponent.h>

#include <Containers/ContainerMacros.h>
#include <Messages/Messages.h>

namespace LockedDoor
{

static const float kOpenAnimKFDuration = 0.15f;

static const Fragment kLeftTPiece	( gCastUCharToChar(195),	ETextDarkYellow );
static const Fragment kRightTPiece	( gCastUCharToChar(180),	ETextDarkYellow );
static const Fragment kTopTPiece	( gCastUCharToChar(194),	ETextDarkYellow );
static const Fragment kBottomTPiece	( gCastUCharToChar(193),	ETextDarkYellow );
static const Fragment kVertiPiece	( gCastUCharToChar(179),	ETextDarkYellow );
static const Fragment kHorizPiece	( gCastUCharToChar(196),	ETextDarkYellow );
static const Fragment kLockPiece	( 'o',						ETextDarkYellow );

enum EAnimationSlot
{
	EAnimationSlot_Idle,
	EAnimationSlot_Open
};

namespace FaceUpDown
{
	static const Fragment	kIdleRenderMeshData[] = { kLeftTPiece, kHorizPiece, kHorizPiece, kLockPiece, kHorizPiece, kHorizPiece, kRightTPiece }; // "|--o--|";
	static const AsciiMesh	kIdleRenderMesh( kIdleRenderMeshData, 7, 1 );
	static const Animation	kIdleAnimation( &kIdleRenderMesh, 1, 0.0f, Animation::EPlaybackStyle_Once );
	
	static const Fragment	kOpenRenderMeshKF1[] = { kLeftTPiece, kHorizPiece,	kHorizPiece,	' ', kHorizPiece,	kHorizPiece,	kRightTPiece }; // "|-- --|";
	static const Fragment	kOpenRenderMeshKF2[] = { kLeftTPiece, kHorizPiece,	' ',			' ', ' ',			kHorizPiece,	kRightTPiece }; // "|-   -|";
	static const Fragment	kOpenRenderMeshKF3[] = { kLeftTPiece, ' ',			' ',			' ', ' ',			' ',			kRightTPiece }; // "|-   -|"; // t pieces
	static const Fragment	kOpenRenderMeshKF4[] = { kVertiPiece, ' ',			' ',			' ', ' ',			' ',			kVertiPiece };	// "|     |";
	
	static const AsciiMesh	kOpenAnimKeyFrames[] =
	{
		AsciiMesh( kOpenRenderMeshKF1,	7, 1 ),
		AsciiMesh( kOpenRenderMeshKF2,	7, 1 ),
		AsciiMesh( kOpenRenderMeshKF3,	7, 1 ),
		AsciiMesh( kOpenRenderMeshKF4,	7, 1 ),
	};

	static const Animation kOpenAnimation(	kOpenAnimKeyFrames, 
											gElemCount(kOpenAnimKeyFrames), 
											kOpenAnimKFDuration, 
											Animation::EPlaybackStyle_Once );
	static const Animation kAnimations[] =
	{
		kIdleAnimation,
		kOpenAnimation,
	};

	static const bool			kIdleCollisionMeshData[]	= { true, true, true, true, true, true, true };
	static const bool			kOpenCollisionMeshData[]	= { true, false, false, false, false, false, true };
	static const CollisionMesh	kIdleCollisionMesh(kIdleCollisionMeshData,	7, 1);
	static const CollisionMesh	kOpenCollisionMesh(kOpenCollisionMeshData,	7, 1);
}

namespace FaceLeftRight
{
	static const Fragment kIdleRenderMeshData[] =
	{
		kTopTPiece,
		kVertiPiece,
		kLockPiece,
		kVertiPiece,
		kBottomTPiece,
	};

	static const AsciiMesh kIdleRenderMesh(kIdleRenderMeshData, 1, 5);
	static const Animation kIdleAnimation( &kIdleRenderMesh, 1, 0.0f, Animation::EPlaybackStyle_Once );
	
	static const Fragment kOpenRenderMeshKF1[] = 
	{
		kTopTPiece,
		kVertiPiece,
		' ',
		kVertiPiece,
		kBottomTPiece,
	};
	static const Fragment kOpenRenderMeshKF2[] = 
	{
		kTopTPiece,
		' ',
		' ',
		' ',
		kBottomTPiece,
	};
	static const Fragment kOpenRenderMeshKF3[] = 
	{
		kHorizPiece,
		' ',
		' ',
		' ',
		kHorizPiece,
	};
	
	static const AsciiMesh kOpenAnimKeyFrames[] =
	{
		AsciiMesh( kOpenRenderMeshKF1,	1, 5 ),
		AsciiMesh( kOpenRenderMeshKF2,	1, 5 ),
		AsciiMesh( kOpenRenderMeshKF3,	1, 5 ),
	};

	static const Animation kOpenAnimation(	kOpenAnimKeyFrames, 
											gElemCount(kOpenAnimKeyFrames), 
											kOpenAnimKFDuration, 
											Animation::EPlaybackStyle_Once );
	static const Animation kAnimations[] =
	{
		kIdleAnimation,
		kOpenAnimation,
	};

	static const bool			kIdleCollisionMeshData[]	= { true, true, true, true, true };
	static const bool			kOpenCollisionMeshData[]	= { true, false, false, false, true };
	static const CollisionMesh	kIdleCollisionMesh(kIdleCollisionMeshData,	1, 5);
	static const CollisionMesh	kOpenCollisionMesh(kOpenCollisionMeshData,	1, 5);
}

class LockedDoorProperties
{
public:
	LockedDoorProperties(bool inIsLeftRightFacing) : mIsLeftRightFacing(inIsLeftRightFacing) { }
	~LockedDoorProperties() { }

	Entity	mOtherDoorBoundTo;
	bool	mIsLeftRightFacing;
};

static void sUnlockDoor(Entity inEntity)
{
	bool	isLeftRightFacing	= inEntity.GetComponent<LockedDoorProperties>()->mIsLeftRightFacing;
	auto&	openCollision		= isLeftRightFacing ? FaceLeftRight::kOpenCollisionMesh : FaceUpDown::kOpenCollisionMesh;

	inEntity.GetComponent<CollisionComponent>()->SetDefaultCollisionMesh(openCollision);
	inEntity.GetComponent<AnimationComponent>()->SetSelectedAnimation(EAnimationSlot_Open, false);
}

Entity Create(World& inWorld, const IVec2& inPosition, EOrientation inOrientation)
{
	auto entity = inWorld.CreateEntity();

	bool	isLeftRight = (inOrientation == EOrientation_FaceLeft) || (inOrientation == EOrientation_FaceRight);
	
	auto&	animations		= isLeftRight ? FaceLeftRight::kAnimations			: FaceUpDown::kAnimations;
	auto&	idleCollision	= isLeftRight ? FaceLeftRight::kIdleCollisionMesh	: FaceUpDown::kIdleCollisionMesh;
	
	entity.AddComponent<AnimationComponent>(animations, gElemCount(animations))->SetSelectedAnimation(EAnimationSlot_Idle);
	entity.AddComponent<CollisionComponent>(idleCollision);
	entity.AddComponent<PositionComponent>(inPosition);
	entity.AddComponent<RenderableComponent>();
	entity.AddComponent<LockedDoorProperties>(isLeftRight);

	IVec2 keyHolePosition = inPosition + (isLeftRight ? IVec2(0, 2) : IVec2(3, 0));

	entity.AddComponent<MessageReceiverComponent>()->Register<UnlockDoorMsg>( [=] (const UnlockDoorMsg& inMsg)
	{
		if (inMsg.mPosition == keyHolePosition)
		{
			sUnlockDoor(entity);

			auto properties = entity.GetComponent<LockedDoorProperties>();
			if (properties->mOtherDoorBoundTo.IsValid())
			{
				sUnlockDoor(properties->mOtherDoorBoundTo);
			}
		}
	} );

	return entity;
}

void BindDoors(const Entity& inEntity0, const Entity& inEntity1)
{
	assert( inEntity0.IsValid() && inEntity1.IsValid() && inEntity0 != inEntity1 );
	assert( inEntity0.HasComponent<LockedDoorProperties>() );
	assert( inEntity1.HasComponent<LockedDoorProperties>() );
	assert( inEntity0.GetComponent<LockedDoorProperties>()->mOtherDoorBoundTo.IsValid() == false );
	assert( inEntity1.GetComponent<LockedDoorProperties>()->mOtherDoorBoundTo.IsValid() == false );

	inEntity0.GetComponent<LockedDoorProperties>()->mOtherDoorBoundTo = inEntity1;
	inEntity1.GetComponent<LockedDoorProperties>()->mOtherDoorBoundTo = inEntity0;
}

}
