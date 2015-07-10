#include "MonsterEntityFactory.h"

#include <EntityComponentSystem/World/World.h>
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
	
	static const char kDeathKF0 = gCastUCharToChar(178);
	static const char kDeathKF1 = gCastUCharToChar(177);
	static const char kDeathKF2 = gCastUCharToChar(176);

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

class WormMonster
{
public:
	WormMonster();
	~WormMonster() { }
	
	void	Update(Entity inThis, float inFrameTime, MessageBroadcaster& inMsgBroadcaster);
	void	TakeDamage(Entity inThis);
	void	OnEntityCollidedWith(Entity inThis, Entity inCollidingEntity);

private:
	IVec2	GetIntendedMovement(float inFrameTime);
	void	ResetMovementDuration();

	float	mMovementCooldownTime;

	float	mTimeUntilDeath;
	bool	mIsDying;
};

WormMonster::WormMonster() 
	: mIsDying			(false)
	, mTimeUntilDeath	(FLT_MAX)
{
	ResetMovementDuration();
}

void WormMonster::Update(Entity inThis, float inFrameTime, MessageBroadcaster& inMsgBroadcaster)
{
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

	// Get intended movement
	// If moving, attack in intended direction.
	// If it's not collidable, or is monster or player, then move there.

	IVec2 intendedMovement = GetIntendedMovement(inFrameTime);
	if (intendedMovement == IVec2(0, 0))
	{
		return;
	}

	auto posComp	= inThis.GetComponent<PositionComponent>();
	auto position	= posComp->GetPosition();
	auto newPos		= position + intendedMovement;

	AttackMsg attackMsg(inThis, newPos, intendedMovement, AttackMsg::EEffect_None);
	MessageHelpers::BroadcastMessageToEntitiesAtPosition(*inThis.GetWorld(), inThis, newPos, attackMsg);

	bool isValidPos = !CollisionSystem::CollidesWithAnyEntity(*inThis.GetWorld(), inThis, newPos);

	if (isValidPos)
	{
		posComp->SetPosition(newPos);
	}
}

IVec2 WormMonster::GetIntendedMovement(float inFrameTime)
{
	IVec2 movement(0, 0);

	mMovementCooldownTime -= inFrameTime;

	if (mMovementCooldownTime < 0.0f)
	{
		int direction = rand() % 4;
		switch (direction)
		{
			case 0: movement.mX =  1; break;
			case 1: movement.mX = -1; break;
			case 2: movement.mY =  1; break;
			case 3: movement.mY = -1; break;
		}

		ResetMovementDuration();
	}

	return movement;
}

void WormMonster::ResetMovementDuration()
{
	mMovementCooldownTime = kMovementCooldownDuration + ((rand()%10) * 0.1f);
}

void WormMonster::TakeDamage(Entity inThis)
{
	if (mIsDying)
	{
		return;
	}

	mIsDying		= true;
	mTimeUntilDeath	= kDeathAnimationDuration;

	inThis.GetComponent<AnimationComponent>()->SetSelectedAnimation( MonsterAnimation::EDeathAnimation, true );
}

void WormMonster::OnEntityCollidedWith(Entity inThis, Entity inCollidingEntity)
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

void Create(World& inWorld, MessageBroadcaster& inMsgBroadcaster, const IVec2& inPosition)
{
	auto entity = EntityBuilder(inWorld)
					.AddComponent<AnimationComponent>( MonsterAnimation::kAnimations, gElemCount(MonsterAnimation::kAnimations) )
					.AddComponent<CollisionComponent>( CollisionMesh(0, 0) )
					.AddComponent<MonsterComponent>()
					.AddComponent<WormMonster>()
					.AddComponent<PositionComponent>(inPosition)
					.AddComponent<RenderableComponent>( MonsterAnimation::kIdleFrames[0] )
					.AddComponent<TriggererComponent>()
					.Create();

	entity.AddComponent<MessageReceiverComponent>()->Register<AttackMsg>
	(
		[=] (const AttackMsg&)
		{
			entity.GetComponent<WormMonster>()->TakeDamage(entity); 
		}
	);
	
	entity.AddComponent<TriggerBoxComponent>()->RegisterOnEnterCallback
	(
		[] (const Entity& inThis, const Entity& inEntity)
		{
			inThis.GetComponent<WormMonster>()->OnEntityCollidedWith(inThis, inEntity);
		}
	);

	entity.AddComponent<ProgramComponent>()->RegisterProgram
	(
		[&inMsgBroadcaster] (const Entity& inThis, float inFrameTime)
		{
			inThis.GetComponent<WormMonster>()->Update(inThis, inFrameTime, inMsgBroadcaster); 
		} 
	);
}

}
