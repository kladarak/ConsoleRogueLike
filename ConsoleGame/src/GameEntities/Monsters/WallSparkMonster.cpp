#include "WallSparkMonster.h"

#include <cstdio>

#include <EntityComponentSystem/World/EntityBuilder.h>

#include <EntityComponent/Components/MonsterComponent.h>
#include <EntityComponent/Components/PlayerComponent.h>
#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/ProgramComponent.h>

#include <EntityComponent/Systems/CollisionSystem.h>

#include <Containers/ContainerMacros.h>

#include <Messages/Messages.h>

#include "MonsterBuilder.h"

namespace WallSparkMonster
{

static const AsciiMesh kKeyFrames[] =
{
	AsciiMesh( Fragment(15, ETextYellow) ),
	AsciiMesh( Fragment('*', ETextYellow) ),
};

static const float		kKFTime = 0.1f;
static const Animation	kAnimation(kKeyFrames, gElemCount(kKeyFrames), kKFTime, Animation::EPlaybackStyle_Loop);

static const float		kTimeBetweenMovement = 0.25f;

EOrientation sGetLeftDirection(EOrientation inDirection)
{
	switch (inDirection)
	{
		case EOrientation_FaceUp:		return EOrientation_FaceLeft;
		case EOrientation_FaceDown:		return EOrientation_FaceRight;
		case EOrientation_FaceLeft:		return EOrientation_FaceDown;
		case EOrientation_FaceRight:	return EOrientation_FaceUp;
		default: assert(false);			return inDirection;
	}
}

EOrientation sGetRightDirection(EOrientation inDirection)
{
	switch (inDirection)
	{
		case EOrientation_FaceUp:		return EOrientation_FaceRight;
		case EOrientation_FaceDown:		return EOrientation_FaceLeft;
		case EOrientation_FaceLeft:		return EOrientation_FaceUp;
		case EOrientation_FaceRight:	return EOrientation_FaceDown;
		default: assert(false);			return inDirection;
	}
}

static const bool sCollidesWithEnvironment(World& inWorld, const IVec2& inPosition)
{
	auto collidables = CollisionSystem::GetListofCollidablesAtPosition(inWorld, inPosition);
	
	for (auto entity : collidables)
	{
		if (!entity.HasComponent<PlayerComponent>() && !entity.HasComponent<MonsterComponent>())
		{
			return true;
		}
	}

	return false;
}

class WallSparkMonsterComponent
{
public:
	WallSparkMonsterComponent();
	~WallSparkMonsterComponent() { }

	void			Update(Entity inThis, float inFrameTime, MessageBroadcaster& inMsgBroadcaster);

private:
	static void		sSnapToWallInDirection(Entity inThis, const IVec2& inDirection);

	EOrientation	mLastMovementDirection;
	float			mTimeUntilNextMove;
	bool			mFoundWall;
};

WallSparkMonsterComponent::WallSparkMonsterComponent()
	: mLastMovementDirection((EOrientation) (rand() % EOrientation_Count))
	, mTimeUntilNextMove(0.0f)
	, mFoundWall(false)
{
}

void WallSparkMonsterComponent::Update(Entity inThis, float inFrameTime, MessageBroadcaster&)
{
	if (!mFoundWall)
	{
		// On first update, snap to wall in init'd direction.
		sSnapToWallInDirection(inThis, gGetOrientationVector(mLastMovementDirection));
		mFoundWall = true;
	}

	// Update movement time
	mTimeUntilNextMove -= inFrameTime;
	if (mTimeUntilNextMove > 0.0f)
	{
		return;
	}
	mTimeUntilNextMove += kTimeBetweenMovement;

	// Update movement
	auto&		world			= *inThis.GetWorld();
	auto		positionComp	= inThis.GetComponent<PositionComponent>();
	const IVec2	position		= positionComp->GetPosition();

	// Test left wall: If it's not there, move into it.
	{
		auto leftWallDir		= sGetLeftDirection(mLastMovementDirection);
		auto testLeftWallPos	= position + gGetOrientationVector(leftWallDir);

		if ( !sCollidesWithEnvironment(world, testLeftWallPos) )
		{
			// Wall doesn't exist; move into this space.
			positionComp->SetPosition(testLeftWallPos);
			mLastMovementDirection = leftWallDir;
			return;
		}
	}

	// Otherwise, try to move forward. Keep turning until moving forward is possible.
	auto		nextDir			= mLastMovementDirection;
	auto		nextPos			= position + gGetOrientationVector(nextDir);
	int			attemptCount	= 0;

	while (sCollidesWithEnvironment(world, nextPos) && attemptCount < EOrientation_Count)
	{
		nextDir = sGetRightDirection(nextDir);
		nextPos = position + gGetOrientationVector(nextDir);
		++attemptCount;
	}

	if (attemptCount != EOrientation_Count)
	{
		positionComp->SetPosition(nextPos);
		mLastMovementDirection = nextDir;
	}
}

void WallSparkMonsterComponent::sSnapToWallInDirection(Entity inThis, const IVec2& inDirection)
{
	auto&	world			= *inThis.GetWorld();
	auto	positionComp	= inThis.GetComponent<PositionComponent>();
	IVec2	position		= positionComp->GetPosition();
	
	do
	{
		position += inDirection;
	}
	while ( !sCollidesWithEnvironment(world, position) );

	positionComp->SetPosition(position);
}

Entity Create(World& inWorld, MessageBroadcaster& inMsgBroadcaster, const IVec2& inPosition)
{
	auto entity = MonsterBuilder(inWorld, &inMsgBroadcaster)
					.SetAnimation( kAnimation )
					.SetPosition( inPosition )
					.SetRenderable( kKeyFrames[0] )
					.SetInvulnerable()
					.Create();
	
	entity.AddComponent<WallSparkMonsterComponent>();

	entity.AddComponent<ProgramComponent>()->RegisterProgram
	(
		[&inMsgBroadcaster] (const Entity& inThis, float inFrameTime)
		{
			inThis.GetComponent<WallSparkMonsterComponent>()->Update(inThis, inFrameTime, inMsgBroadcaster); 
		} 
	);

	return entity;
}

}
