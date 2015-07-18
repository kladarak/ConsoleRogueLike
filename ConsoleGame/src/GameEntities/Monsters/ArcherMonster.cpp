#include "ArcherMonster.h"

#include <EntityComponentSystem/World/EntityBuilder.h>

#include <EntityComponent/Components/AnimationComponent.h>
#include <EntityComponent/Components/CollisionComponent.h>
#include <EntityComponent/Components/MessageReceiverComponent.h>
#include <EntityComponent/Components/MonsterComponent.h>
#include <EntityComponent/Components/PlayerComponent.h>
#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/ProgramComponent.h>
#include <EntityComponent/Components/RenderableComponent.h>
#include <EntityComponent/Components/TriggerBoxComponent.h>

#include <EntityComponent/Systems/CollisionSystem.h>

#include <Containers/ContainerMacros.h>

#include <Dungeon/RoomEntity.h>

#include <Inventory/Items/Arrow.h>

#include <Messages/Messages.h>


namespace ArcherMonster
{

static const float kTimeBetweenMovement		= 0.5f;
static const float kTimeBetweenFiringArrows	= 1.0f;
static const float kDeathAnimationDuration	= 1.5f;

static const AsciiMesh kIdleMesh( Fragment('O', ETextRed) );

static const AsciiMesh kDeathFrames[] =
{
	gCastUCharToChar(178),
	gCastUCharToChar(177),
	gCastUCharToChar(176),
	gCastUCharToChar(250),
};
	
static const Animation kDeathAnimation(kDeathFrames, gElemCount(kDeathFrames), kDeathAnimationDuration/gElemCount(kDeathFrames), Animation::EPlaybackStyle_Once);


static int GetSign(int inValue)
{
	return (inValue < 0) ? -1 : ((inValue > 0) ? 1 : 0);
}

class ArcherMonsterComponent
{
public:
	ArcherMonsterComponent();
	~ArcherMonsterComponent() {}

	void Update(Entity inThis, float inFrameTime, MessageBroadcaster&);

	void OnEntityCollidedWith(Entity inThis, Entity inEntity);
	
	void OnPlayerEnteredPatrolArea(Entity inPlayer) { mTarget = inPlayer; }
	void OnPlayerExitedPatrolArea(Entity inPlayer)	{ mTarget = Entity(); }

	void TakeDamage(Entity inThis, Entity inPatrolBoundsEntity); 

private:
	Entity	mTarget;
	float	mTimeSinceLastMovement;
	float	mTimeSinceLastFiredArrow;
	float	mTimeUntilDeath;
	bool	mIsDying;
};

ArcherMonsterComponent::ArcherMonsterComponent()
	: mTimeSinceLastMovement(0.0f)
	, mTimeSinceLastFiredArrow(0.0f)
	, mTimeUntilDeath(FLT_MAX)
	, mIsDying(false)
{
}

void ArcherMonsterComponent::Update(Entity inThis, float inFrameTime, MessageBroadcaster& inMsgBroadcaster)
{
	if (!mTarget.IsValid())
	{
		return;
	}
	
	if (mIsDying)
	{
		mTimeUntilDeath -= inFrameTime;

		if (mTimeUntilDeath <= 0.0f)
		{
			auto position = inThis.GetComponent<PositionComponent>()->GetPosition();
			inThis.Kill();
			inMsgBroadcaster.Broadcast( MonsterDiedMsg(position) );
		}

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
				Arrow::Create(*inThis.GetWorld(), thisPosition, direction);
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

void ArcherMonsterComponent::OnEntityCollidedWith(Entity inThis, Entity inCollidingEntity)
{
	if (mIsDying)
	{
		return;
	}

	auto msgRecComp = inCollidingEntity.GetComponent<MessageReceiverComponent>();
	if (nullptr != msgRecComp)
	{
		auto position = inThis.GetComponent<PositionComponent>()->GetPosition();
		AttackMsg attackMsg(inThis, position, IVec2(0, 0), AttackMsg::EEffect_PushBack);
		msgRecComp->Broadcast( attackMsg );
	}
}

void ArcherMonsterComponent::TakeDamage(Entity inThis, Entity inPatrolBoundsEntity)
{
	mIsDying		= true;
	mTimeUntilDeath = kDeathAnimationDuration;

	inPatrolBoundsEntity.Kill();
	inThis.GetComponent<AnimationComponent>()->SetAnimation( kDeathAnimation );
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

	auto archerEntity = EntityBuilder(inWorld)
							.AddComponent<AnimationComponent>( Animation(&kIdleMesh, 1, 0.0f, Animation::EPlaybackStyle_Loop) )
							.AddComponent<CollisionComponent>( CollisionMesh(0, 0) )
							.AddComponent<MonsterComponent>()
							.AddComponent<ArcherMonsterComponent>()
							.AddComponent<PositionComponent>(inPosition)
							.AddComponent<RenderableComponent>( kIdleMesh )
							.Create();

	{
		auto patrolTriggerBoxComp = patrolBoundsEntity.GetComponent<TriggerBoxComponent>();
		patrolTriggerBoxComp->RegisterOnEnterCallback
		(
			[=] (const Entity& inThis, const Entity& inEntity)
			{
				if (inEntity.HasComponent<PlayerComponent>())
				{
					archerEntity.GetComponent<ArcherMonsterComponent>()->OnPlayerEnteredPatrolArea(inEntity);
				}
			}
		);

		patrolTriggerBoxComp->RegisterOnExitCallback
		(
			[=] (const Entity& inThis, const Entity& inEntity)
			{
				if (inEntity.HasComponent<PlayerComponent>())
				{
					archerEntity.GetComponent<ArcherMonsterComponent>()->OnPlayerExitedPatrolArea(inEntity);
				}
			}
		);
	}
	
	archerEntity.AddComponent<TriggerBoxComponent>()->RegisterOnEnterCallback
	(
		[] (const Entity& inThis, const Entity& inEntity)
		{
			inThis.GetComponent<ArcherMonsterComponent>()->OnEntityCollidedWith(inThis, inEntity);
		}
	);

	archerEntity.AddComponent<ProgramComponent>()->RegisterProgram
	(
		[&inMsgBroadcaster] (const Entity& inThis, float inFrameTime)
		{
			inThis.GetComponent<ArcherMonsterComponent>()->Update(inThis, inFrameTime, inMsgBroadcaster); 
		} 
	);
	
	archerEntity.AddComponent<MessageReceiverComponent>()->Register<AttackMsg>
	(
		[=] (const AttackMsg&)
		{
			// TODO: archer entity needs to destroy its patrol trigger box entity too.
			archerEntity.GetComponent<ArcherMonsterComponent>()->TakeDamage(archerEntity, patrolBoundsEntity); 
		}
	);

	return archerEntity;
}

}