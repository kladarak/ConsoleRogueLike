#include "MonsterEntityFactory.h"

#include <Core/Messaging/MessageBroadcaster.h>

#include <EntityComponentSystem/World/World.h>

#include <EntityComponent/Components/AnimationComponent.h>
#include <EntityComponent/Components/CollisionComponent.h>
#include <EntityComponent/Components/PlayerComponent.h>
#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/ProgramComponent.h>
#include <EntityComponent/Components/RenderableComponent.h>
#include <EntityComponent/Components/TriggerBoxComponent.h>

#include <EntityComponent/Systems/CollisionSystem.h>

#include <GameEntities/CoinEntity.h>
#include <GameEntities/HealthEntity.h>

#include <Messages/Messages.h>

namespace MonsterEntityFactory
{

static const float	kAnimationDuration			= 0.5f;
static const float	kMovementCooldownDuration	= 1.0f;

static const AsciiMesh kMonsterFrames[] =
{
	AsciiMesh( "e", 1, 1 ),
	AsciiMesh( "@", 1, 1 ),
};

static const Animation kMonsterAnimation(kMonsterFrames, sizeof(kMonsterFrames)/sizeof(AsciiMesh), kAnimationDuration, Animation::EPlaybackStyle_Loop);

class MonsterState
{
public:
	MonsterState()
	{
		ResetMovementDuration();
	}

	MonsterState(MonsterState&& inState)
		: mMovementCooldownTime				(inState.mMovementCooldownTime)
		, mPlayerAttackMsgCallbackRegHandle	(inState.mPlayerAttackMsgCallbackRegHandle)
	{
		inState.mPlayerAttackMsgCallbackRegHandle = MessageRegistrationHandle();
	}

	~MonsterState()
	{
		mPlayerAttackMsgCallbackRegHandle.Unregister();
	}

	void ResetMovementDuration()
	{
		mMovementCooldownTime = kMovementCooldownDuration + ((rand()%10) * 0.1f);
	}

	float						mMovementCooldownTime;
	MessageRegistrationHandle	mPlayerAttackMsgCallbackRegHandle;
};

static void Update(const Entity& inThis, float inFrameTime)
{
	auto state = inThis.GetComponent<MonsterState>();
	state->mMovementCooldownTime -= inFrameTime;

	if (state->mMovementCooldownTime < 0.0f)
	{
		auto posComp = inThis.GetComponent<PositionComponent>();
		auto position = posComp->GetPosition();

		int direction = rand() % 4;
		switch (direction)
		{
			case 0: position.mX += 1; break;
			case 1: position.mX -= 1; break;
			case 2: position.mY += 1; break;
			case 3: position.mY -= 1; break;
		}

		if (!CollisionSystem::CollidesWithAnyEntity(*inThis.GetWorld(), inThis, position))
		{
			posComp->SetPosition(position);
		}

		state->ResetMovementDuration();
	}
}

static void OnEntityEntered(const Entity& inMonster, const Entity& inTriggerer, MessageBroadcaster& inMsgBroadcaster)
{
	auto playerComponent = inTriggerer.GetComponent<PlayerComponent>();

	if ( nullptr == playerComponent )
	{
		return;
	}

	inMsgBroadcaster.Broadcast( TouchedMonsterMsg(inMonster, inTriggerer) );
}

static void OnPlayerAttack(Entity inMonster, const PlayerAttackMsg& inAttackMsg, MessageBroadcaster& inMsgBroadcaster)
{
	auto position = inMonster.GetComponent<PositionComponent>()->GetPosition();
	if (inAttackMsg.mAttackPosition == position)
	{
		Entity(inMonster).Kill();
		switch ((rand() % 3) == 0)
		{
			case 0: CoinEntity::Create(*inMonster.GetWorld(), inMsgBroadcaster, position); break;
			case 1: HealthEntity::Create(*inMonster.GetWorld(), inMsgBroadcaster, position); break;
			default: break;
		}
	}
}

void Create(World& inWorld, MessageBroadcaster& inMsgBroadcaster, const IVec2& inPosition)
{
	auto entity = inWorld.CreateEntity();

	entity.AddComponent<PositionComponent>(inPosition);
	entity.AddComponent<ProgramComponent>()->RegisterProgram( &Update );
	entity.AddComponent<AnimationComponent>(kMonsterAnimation);
	entity.AddComponent<RenderableComponent>(kMonsterFrames[0]);

	auto triggerBox = entity.AddComponent<TriggerBoxComponent>( IRect(0, 0, 1, 1) );
	triggerBox->RegisterOnEnterCallback( [&] (const Entity& inMonster, const Entity& inTriggerer)
	{
		OnEntityEntered(inMonster, inTriggerer, inMsgBroadcaster);
	} );

	auto registrationHandle = inMsgBroadcaster.Register<PlayerAttackMsg>( [entity, &inMsgBroadcaster] (const PlayerAttackMsg& inAttackMsg) 
	{
		OnPlayerAttack(entity, inAttackMsg, inMsgBroadcaster); 
	} );

	entity.AddComponent<MonsterState>()->mPlayerAttackMsgCallbackRegHandle = registrationHandle;
}

}
