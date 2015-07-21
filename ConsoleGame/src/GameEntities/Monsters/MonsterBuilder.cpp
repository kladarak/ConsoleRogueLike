#include "MonsterBuilder.h"

#include <EntityComponentSystem/World/EntityBuilder.h>

#include <EntityComponent/Components/AnimationComponent.h>
#include <EntityComponent/Components/CollisionComponent.h>
#include <EntityComponent/Components/MessageReceiverComponent.h>
#include <EntityComponent/Components/MonsterComponent.h>
#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/ProgramComponent.h>
#include <EntityComponent/Components/RenderableComponent.h>
#include <EntityComponent/Components/TriggerBoxComponent.h>

#include <EntityComponent/Systems/CollisionSystem.h>

#include <Messages/Messages.h>


MonsterBuilder::MonsterBuilder(World& inWorld, MessageBroadcaster* inMsgBroadcaster)
	: mWorld			(inWorld)
	, mMsgBroadcaster	(inMsgBroadcaster)
	, mRenderMesh		(Fragment('!', EColour_Background_Red))
	, mPosition			(0, 0)
	, mCanBeKilled		(true)
{
}

MonsterBuilder&	MonsterBuilder::SetAnimation(const Animation& inAnimation)
{
	mAnimation = inAnimation;
	mOptionalComponents |= EAnimation;
	return *this;
}

MonsterBuilder&	MonsterBuilder::SetPosition(const IVec2& inPosition)
{
	mPosition = inPosition;
	return *this;
}
	
MonsterBuilder&	MonsterBuilder::SetRenderable(const AsciiMesh& inAsciiMesh)
{
	mRenderMesh = inAsciiMesh;

	if ((mOptionalComponents & EAnimation) == 0)
	{
		mAnimation = Animation(&mRenderMesh, 1, 0.0f, Animation::EPlaybackStyle_Loop);
	}

	return *this;
}

MonsterBuilder& MonsterBuilder::SetInvulnerable()
{
	mCanBeKilled = false;
	return *this;
}

Entity MonsterBuilder::Create()
{
	EntityBuilder builder(mWorld);
	
	builder.AddComponent<CollisionComponent>( CollisionMesh(0, 0) )
			.AddComponent<MonsterComponent>()
			.AddComponent<PositionComponent>( mPosition )
			.AddComponent<RenderableComponent>( mRenderMesh )
			.AddComponent<TriggererComponent>()
			.AddComponent<AnimationComponent>( mAnimation );

	auto entity = builder.Create();

	if (mCanBeKilled)
	{
		entity.AddComponent<MessageReceiverComponent>()->Register<AttackMsg>
		(
			[=] (const AttackMsg&)
			{
				entity.GetComponent<MonsterComponent>()->StartDeath(entity); 
			}
		);
		
		MessageBroadcaster* broadcaster = mMsgBroadcaster;

		entity.AddComponent<ProgramComponent>()->RegisterProgram
		(
			[=] (const Entity& inThis, float inFrameTime)
			{
				inThis.GetComponent<MonsterComponent>()->Update(inThis, inFrameTime, *broadcaster); 
			} 
		);
	}
	
	entity.AddComponent<TriggerBoxComponent>()->RegisterOnEnterCallback
	(
		[] (const Entity& inThis, const Entity& inEntity)
		{
			inThis.GetComponent<MonsterComponent>()->OnEntityCollidedWith(inThis, inEntity);
		}
	);

	
	return entity;
}