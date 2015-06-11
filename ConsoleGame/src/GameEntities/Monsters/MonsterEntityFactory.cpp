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

static const char	kMonsterAnimation[]	= "e@";
static const float	kAnimationDuration	= 0.5f;

class MonsterState
{
public:
	MonsterState()
		: mAnimationTime(0.0f)
		, mAnimationFrame(0)
	{
	}

	MonsterState(MonsterState&& inState)
		: mAnimationTime					(inState.mAnimationTime)
		, mAnimationFrame					(inState.mAnimationFrame)
		, mPlayerAttackMsgCallbackRegHandle	(inState.mPlayerAttackMsgCallbackRegHandle)
	{
		inState.mPlayerAttackMsgCallbackRegHandle = MessageRegistrationHandle();
	}

	~MonsterState()
	{
		mPlayerAttackMsgCallbackRegHandle.Unregister();
	}

	float						mAnimationTime;
	int							mAnimationFrame;

	MessageRegistrationHandle	mPlayerAttackMsgCallbackRegHandle;
};

static void Update(const Entity& inThis, float inFrameTime)
{
	auto state = inThis.GetComponent<MonsterState>();
	state->mAnimationTime += inFrameTime;

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
