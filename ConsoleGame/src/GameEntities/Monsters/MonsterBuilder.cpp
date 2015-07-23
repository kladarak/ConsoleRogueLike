#include "MonsterBuilder.h"

#include <EntityComponentSystem/World/EntityBuilder.h>

#include <EntityComponent/Components/AnimationComponent.h>
#include <EntityComponent/Components/CollisionComponent.h>
#include <EntityComponent/Components/HealthComponent.h>
#include <EntityComponent/Components/MessageReceiverComponent.h>
#include <EntityComponent/Components/MonsterComponent.h>
#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/ProgramComponent.h>
#include <EntityComponent/Components/RenderableComponent.h>
#include <EntityComponent/Components/TriggerBoxComponent.h>

#include <EntityComponent/Systems/CollisionSystem.h>

#include <Messages/Messages.h>


MonsterBuilder::MonsterBuilder(World& inWorld, MessageBroadcaster* inMsgBroadcaster)
	: mWorld				(inWorld)
	, mMsgBroadcaster		(inMsgBroadcaster)
	, mRenderMesh			(Fragment('!', EColour_Background_Red))
	, mPosition				(0, 0)
	, mHealthAmount			(4) // Default, so that a standard attack will kill it, a weak attack will not.
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
	mHealthAmount = 0;
	return *this;
}

MonsterBuilder& MonsterBuilder::SetHealthAmount(int inAmount)
{
	mHealthAmount = inAmount;
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

	if (mHealthAmount > 0)
	{
		entity.AddComponent<HealthComponent>(mHealthAmount);

		entity.AddComponent<MessageReceiverComponent>()->Register<AttackMsg>
		(
			[=] (const AttackMsg& inMsg)
			{
				auto healthComp = entity.GetComponent<HealthComponent>();
				
				if (!healthComp->IsDead())
				{
					healthComp->DecHealth(inMsg.mDamage);

					if (healthComp->IsDead())
					{
						entity.GetComponent<MonsterComponent>()->StartDeath(entity); 
					}
					else
					{
						entity.GetComponent<MonsterComponent>()->StartDamagedAnimation(); 
					}
				}
			}
		);
		
		MessageBroadcaster* broadcaster = mMsgBroadcaster;

		entity.AddComponent<ProgramComponent>()->RegisterProgram
		(
			[=] (const Entity& inThis, float inFrameTime)
			{
				// Badly named function: Update updates its time until death.
				inThis.GetComponent<MonsterComponent>()->UpdateDamageAnimations(inThis, inFrameTime, *broadcaster); 
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