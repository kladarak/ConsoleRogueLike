#include "MonsterEntityFactory.h"

#include <EntityComponentSystem/World/World.h>

#include <EntityComponent/Components/AnimationComponent.h>
#include <EntityComponent/Components/CollisionComponent.h>
#include <EntityComponent/Components/DamageZoneComponent.h>
#include <EntityComponent/Components/MessageReceiverComponent.h>
#include <EntityComponent/Components/MonsterComponent.h>
#include <EntityComponent/Components/PlayerComponent.h>
#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/ProgramComponent.h>
#include <EntityComponent/Components/RenderableComponent.h>
#include <EntityComponent/Components/TriggerBoxComponent.h>

#include <EntityComponent/Systems/CollisionSystem.h>
#include <EntityComponent/Systems/DamageZoneSystem.h>

#include <GameEntities/CoinEntity.h>
#include <GameEntities/HealthEntity.h>

#include <Containers/ContainerMacros.h>

#include <Messages/Messages.h>

namespace MonsterEntityFactory
{

static const float	kAnimationDuration			= 0.5f;
static const float	kMovementCooldownDuration	= 1.0f;
static const float	kDeathAnimationDuration		= 1.5f;

namespace MonsterAnimation
{
	static const AsciiMesh kIdleFrames[] =
	{
		AsciiMesh( "e", 1, 1 ),
		AsciiMesh( "@", 1, 1 ),
	};

	static const Animation kIdle(kIdleFrames, gElemCount(kIdleFrames), kAnimationDuration, Animation::EPlaybackStyle_Loop);
	
	static const char kDeathKF0 = 178;
	static const char kDeathKF1 = 177;
	static const char kDeathKF2 = 176;

	static const AsciiMesh kDeathFrames[] =
	{
		AsciiMesh( &kDeathKF0,	1, 1 ),
		AsciiMesh( &kDeathKF1,	1, 1 ),
		AsciiMesh( &kDeathKF2,	1, 1 ),
		AsciiMesh( ".",			1, 1 ),
	};
	
	static const Animation kDeath(kDeathFrames, gElemCount(kDeathFrames), kDeathAnimationDuration/gElemCount(kDeathFrames), Animation::EPlaybackStyle_Once);

	static const Animation kAnimations[] =
	{
		kIdle,
		kDeath
	};

	enum
	{
		EIdleAnimation = 0,
		EDeathAnimation
	};
}

class MonsterState
{
public:
	MonsterState() : mIsDying(false), mTimeUntilDeath(FLT_MAX)
	{
		ResetMovementDuration();
	}

	~MonsterState() { }

	void ResetMovementDuration()
	{
		mMovementCooldownTime = kMovementCooldownDuration + ((rand()%10) * 0.1f);
	}

	float	mMovementCooldownTime;

	float	mTimeUntilDeath;
	bool	mIsDying;
};

static void TakeDamage(Entity inMonster)
{
	auto monsterState = inMonster.GetComponent<MonsterState>();
	if (monsterState->mIsDying)
	{
		return;
	}

	monsterState->mIsDying			= true;
	monsterState->mTimeUntilDeath	= kDeathAnimationDuration;

	inMonster.GetComponent<AnimationComponent>()->SetSelectedAnimation( MonsterAnimation::EDeathAnimation, true );
	inMonster.RemoveComponent<DamageZoneComponent>();
}

static void OnAttacked(Entity inMonster, const AttackMsg&)
{
	TakeDamage(inMonster);
}

static IVec2 GetIntendedMovement(const Entity& inThis, float inFrameTime)
{
	IVec2 movement(0, 0);

	auto state = inThis.GetComponent<MonsterState>();
	state->mMovementCooldownTime -= inFrameTime;

	if (state->mMovementCooldownTime < 0.0f)
	{
		int direction = rand() % 4;
		switch (direction)
		{
			case 0: movement.mX =  1; break;
			case 1: movement.mX = -1; break;
			case 2: movement.mY =  1; break;
			case 3: movement.mY = -1; break;
		}

		state->ResetMovementDuration();
	}

	return movement;
}

static void Update(Entity inThis, float inFrameTime, MessageBroadcaster& inMsgBroadcaster)
{
	auto monsterState = inThis.GetComponent<MonsterState>();
	if (monsterState->mIsDying)
	{
		monsterState->mTimeUntilDeath -= inFrameTime;

		if (monsterState->mTimeUntilDeath <= 0.0f)
		{
			auto position = inThis.GetComponent<PositionComponent>()->GetPosition();

			inThis.Kill();

			switch (rand() % 3)
			{
				case 0: CoinEntity::Create(*inThis.GetWorld(), inMsgBroadcaster, position); break;
				case 1: HealthEntity::Create(*inThis.GetWorld(), position); break;
				default: break;
			}
		}

		return;
	}

	// Check if in danger zone.
	// If so, take damage.
	// If not, get intended movement
	// If moving, attack in intended direction.
	// If it's not collidable, or is monster or player, then move there.

	auto posComp = inThis.GetComponent<PositionComponent>();
	auto position = posComp->GetPosition();

	{
		bool isInDamageZone = DamageZoneSystem::IsDamageZone(*inThis.GetWorld(), inThis, position);
		if (isInDamageZone)
		{
			TakeDamage(inThis);
			return;
		}
	}

	IVec2 intendedMovement = GetIntendedMovement(inThis, inFrameTime);
	if (intendedMovement == IVec2(0, 0))
	{
		return;
	}

	auto newPos = position + intendedMovement;

	AttackMsg attackMsg(inThis, newPos, intendedMovement);
	MessageHelpers::BroadcastMessageToEntitiesAtPosition(*inThis.GetWorld(), inThis, newPos, attackMsg);

	bool isValidPos = true;
	
	auto collidablesAtPosition = CollisionSystem::GetListofCollidablesAtPosition(*inThis.GetWorld(), newPos);
	for (auto& collidable : collidablesAtPosition)
	{
		if (!collidable.HasComponent<MonsterComponent>() && !collidable.HasComponent<PlayerComponent>())
		{
			isValidPos = false;
			break;
		}
	}

	if (isValidPos)
	{
		posComp->SetPosition(newPos);
	}
}

void Create(World& inWorld, MessageBroadcaster& inMsgBroadcaster, const IVec2& inPosition)
{
	auto entity = inWorld.CreateEntity();
	
	entity.AddComponent<AnimationComponent>( MonsterAnimation::kAnimations, gElemCount(MonsterAnimation::kAnimations) );
	entity.AddComponent<CollisionComponent>()->SetCollidableAt(0, 0);
	entity.AddComponent<DamageZoneComponent>()->SetDamageZoneAt(0, 0);
	entity.AddComponent<MonsterComponent>();
	entity.AddComponent<MonsterState>();
	entity.AddComponent<PositionComponent>(inPosition);
	entity.AddComponent<RenderableComponent>( MonsterAnimation::kIdleFrames[0] );

	entity.AddComponent<MessageReceiverComponent>()->Register<AttackMsg>(
		[entity, &inMsgBroadcaster] (const AttackMsg& inAttackMsg)
		{
			OnAttacked(entity, inAttackMsg); 
		} );
	
	entity.AddComponent<ProgramComponent>()->RegisterProgram(
		[&inMsgBroadcaster] (const Entity& inThis, float inFrameTime)
		{
			Update(inThis, inFrameTime, inMsgBroadcaster); 
		} );
}

}
