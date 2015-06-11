#include "MonsterEntityFactory.h"

#include <Core/Messaging/MessageBroadcaster.h>

#include <EntityComponentSystem/World/World.h>

#include <EntityComponent/Components/CollisionComponent.h>
#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/RenderableComponent.h>
#include <EntityComponent/Components/TriggerBoxComponent.h>
#include <EntityComponent/Components/PlayerComponent.h>
#include <EntityComponent/Components/ProgramComponent.h>

#include <EntityComponent/Systems/CollisionSystem.h>

#include <Messages/Messages.h>

namespace MonsterEntityFactory
{

static const char	kMonsterAnimation[]			= "e@";
static const float	kAnimationDuration			= 0.5f;
static const float	kMovementCooldownDuration	= 1.0f;

class MonsterState
{
public:
	MonsterState()
		: mAnimationTime(0.0f)
		, mAnimationFrame(0)
	{
		ResetMovementDuration();
	}

	MonsterState(MonsterState&& inState)
		: mAnimationTime					(inState.mAnimationTime)
		, mAnimationFrame					(inState.mAnimationFrame)
		, mMovementCooldownTime				(inState.mMovementCooldownTime)
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

	float						mAnimationTime;
	int							mAnimationFrame;

	float						mMovementCooldownTime;

	MessageRegistrationHandle	mPlayerAttackMsgCallbackRegHandle;
};

static void Update(const Entity& inThis, float inFrameTime)
{
	auto state = inThis.GetComponent<MonsterState>();
	state->mMovementCooldownTime	-= inFrameTime;
	state->mAnimationTime			+= inFrameTime;

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

	if (state->mAnimationTime > kAnimationDuration)
	{
		state->mAnimationTime -= kAnimationDuration;
		state->mAnimationFrame = (state->mAnimationFrame + 1) % (sizeof(kMonsterAnimation) - 1);

		AsciiMesh mesh;
		mesh.SetCharAtPosition(0, 0, kMonsterAnimation[state->mAnimationFrame]);
		inThis.GetComponent<RenderableComponent>()->SetMesh(mesh);
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

static void OnPlayerAttack(Entity inMonster, const PlayerAttackMsg& inAttackMsg)
{
	auto position = inMonster.GetComponent<PositionComponent>()->GetPosition();
	if (inAttackMsg.mAttackPosition == position)
	{
		Entity(inMonster).Kill();
	}
}

void Create(World& inWorld, MessageBroadcaster& inMsgBroadcaster, const IVec2& inPosition)
{
	auto entity = inWorld.CreateEntity();

	entity.AddComponent<PositionComponent>(inPosition);
	auto monsterState = entity.AddComponent<MonsterState>();

	auto triggerBox = entity.AddComponent<TriggerBoxComponent>( IRect(0, 0, 1, 1) );
	triggerBox->RegisterOnEnterCallback( [&] (const Entity& inMonster, const Entity& inTriggerer)
	{
		OnEntityEntered(inMonster, inTriggerer, inMsgBroadcaster);
	} );

	auto programComp = entity.AddComponent<ProgramComponent>();
	programComp->RegisterProgram( &Update );

	AsciiMesh mesh;
	mesh.SetCharAtPosition(0, 0, kMonsterAnimation[0]);
	entity.AddComponent<RenderableComponent>(mesh);

	auto registrationHandle = inMsgBroadcaster.Register<PlayerAttackMsg>( [entity] (const PlayerAttackMsg& inAttackMsg) 
	{
		OnPlayerAttack(entity, inAttackMsg); 
	} );

	monsterState->mPlayerAttackMsgCallbackRegHandle = registrationHandle;
}

}
