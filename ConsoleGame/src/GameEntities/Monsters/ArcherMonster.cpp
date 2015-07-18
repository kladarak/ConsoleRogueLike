#include "ArcherMonster.h"

#include <EntityComponentSystem/World/EntityBuilder.h>

#include <EntityComponent/Components/MonsterComponent.h>
#include <EntityComponent/Components/PlayerComponent.h>
#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/ProgramComponent.h>
#include <EntityComponent/Components/TriggerBoxComponent.h>

#include <EntityComponent/Systems/CollisionSystem.h>

#include <Containers/ContainerMacros.h>

#include <Dungeon/RoomEntity.h>

#include <Inventory/Items/Arrow.h>

#include <Messages/Messages.h>

#include "MonsterBuilder.h"

namespace ArcherMonster
{

static const float kTimeBetweenMovement		= 0.5f;
static const float kTimeBetweenFiringArrows	= 1.0f;

static const AsciiMesh kIdleMesh( Fragment('O', ETextRed) );
static const Animation kIdleAnimation(&kIdleMesh, 1, 0.0f, Animation::EPlaybackStyle_Loop);

static int GetSign(int inValue)
{
	return (inValue < 0) ? -1 : ((inValue > 0) ? 1 : 0);
}

class ArcherMonsterComponent
{
public:
	ArcherMonsterComponent(const Entity& inPatrolBoundsEntity);
	ArcherMonsterComponent(ArcherMonsterComponent&& inOther);
	~ArcherMonsterComponent() { }

	void Update(Entity inThis, float inFrameTime);

	void OnPlayerEnteredPatrolArea(Entity inPlayer) { mTarget = inPlayer; }
	void OnPlayerExitedPatrolArea(Entity)			{ mTarget = Entity(); }

private:
	Entity	mPatrolBoundsEntity; 
	Entity	mTarget;
	float	mTimeSinceLastMovement;
	float	mTimeSinceLastFiredArrow;
};

ArcherMonsterComponent::ArcherMonsterComponent(const Entity& inPatrolBoundsEntity)
	: mPatrolBoundsEntity(inPatrolBoundsEntity)
	, mTimeSinceLastMovement(0.0f)
	, mTimeSinceLastFiredArrow(0.0f)
{
}

ArcherMonsterComponent::ArcherMonsterComponent(ArcherMonsterComponent&& inOther)
	: mPatrolBoundsEntity		(inOther.mPatrolBoundsEntity)
	, mTarget					(inOther.mTarget)
	, mTimeSinceLastMovement	(inOther.mTimeSinceLastMovement)
	, mTimeSinceLastFiredArrow	(inOther.mTimeSinceLastFiredArrow)
{
	inOther.mPatrolBoundsEntity = Entity();
	inOther.mTarget				= Entity();
}

void ArcherMonsterComponent::Update(Entity inThis, float inFrameTime)
{
	if (inThis.GetComponent<MonsterComponent>()->IsDying())
	{
		if (mPatrolBoundsEntity.IsAlive())
		{
			mPatrolBoundsEntity.Kill();
		}

		return;
	}

	if (!mTarget.IsValid())
	{
		return;
	}

	auto thisPosition	= inThis.GetComponent<PositionComponent>()->GetPosition();
	auto targetPosition = mTarget.GetComponent<PositionComponent>()->GetPosition();
	auto diffPos		= targetPosition - thisPosition;
	
	if (diffPos.mX == 0 || diffPos.mY == 0)
	{
		mTimeSinceLastFiredArrow += inFrameTime;

		if (mTimeSinceLastFiredArrow >= kTimeBetweenFiringArrows)
		{
			mTimeSinceLastFiredArrow = 0;

			IVec2 direction(0, 0);

			if (diffPos.mX == 0)
			{
				direction.mY = GetSign(diffPos.mY);
			}
			else
			{
				direction.mX = GetSign(diffPos.mX);
			}

			if (direction.mX != 0 || direction.mY != 0)
			{
				IVec2 startPosition = thisPosition + direction;
				Arrow::Create(*inThis.GetWorld(), startPosition, direction);
			}
		}
	}
	else 
	{
		mTimeSinceLastMovement += inFrameTime;

		if (mTimeSinceLastMovement >= kTimeBetweenMovement)
		{
			mTimeSinceLastMovement = 0.0f;

			if (abs(diffPos.mX) < abs(diffPos.mY))
			{
				thisPosition.mX += GetSign(diffPos.mX);
			}
			else
			{
				thisPosition.mY += GetSign(diffPos.mY);
			}

			inThis.GetComponent<PositionComponent>()->SetPosition(thisPosition);
		}
	}
}

Entity Create(World& inWorld, MessageBroadcaster& inMsgBroadcaster, const IVec2& inPosition)
{
	int roomCol		= inPosition.mX / ERoomDimensions_Width;
	int roomRow		= inPosition.mY / ERoomDimensions_Height;
	int roomColPos	= roomCol * ERoomDimensions_Width;
	int roomRowPos	= roomRow * ERoomDimensions_Height;

	auto patrolBoundsEntity = EntityBuilder(inWorld)
								.AddComponent<PositionComponent>(roomColPos, roomRowPos)
								.AddComponent<TriggerBoxComponent>(IRect(0, 0, ERoomDimensions_Width, ERoomDimensions_Height))
								.Create();

	auto archerEntity = MonsterBuilder(inWorld, &inMsgBroadcaster)
							.SetRenderable(kIdleMesh)
							.SetPosition(inPosition)
							.Create();

	archerEntity.AddComponent<ArcherMonsterComponent>(patrolBoundsEntity);

	{
		auto patrolTriggerBoxComp = patrolBoundsEntity.GetComponent<TriggerBoxComponent>();
		patrolTriggerBoxComp->RegisterOnEnterCallback
		(
			[=] (const Entity&, const Entity& inEntity)
			{
				if (inEntity.HasComponent<PlayerComponent>())
				{
					archerEntity.GetComponent<ArcherMonsterComponent>()->OnPlayerEnteredPatrolArea(inEntity);
				}
			}
		);

		patrolTriggerBoxComp->RegisterOnExitCallback
		(
			[=] (const Entity&, const Entity& inEntity)
			{
				if (inEntity.HasComponent<PlayerComponent>())
				{
					archerEntity.GetComponent<ArcherMonsterComponent>()->OnPlayerExitedPatrolArea(inEntity);
				}
			}
		);
	}
	
	archerEntity.GetComponent<ProgramComponent>()->RegisterProgram
	(
		[&inMsgBroadcaster] (const Entity& inThis, float inFrameTime)
		{
			inThis.GetComponent<ArcherMonsterComponent>()->Update(inThis, inFrameTime); 
		} 
	);

	return archerEntity;
}

}