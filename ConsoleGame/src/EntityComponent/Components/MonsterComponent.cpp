#include "MonsterComponent.h"

#include <Core/Messaging/MessageBroadcaster.h>

#include <EntityComponentSystem/World/World.h>

#include <EntityComponent/Components/AnimationComponent.h>
#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/MessageReceiverComponent.h>

#include <GameEntities/Monsters/MonsterDeathAnimation.h>

#include <Messages/Messages.h>

MonsterComponent::MonsterComponent()
	: mTimeUntilDeath	(FLT_MAX)
	, mIsDying			(false)
{
}

void MonsterComponent::Update(Entity inThis, float inFrameTime, MessageBroadcaster& inMsgBroadcaster)
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
	}
}

void MonsterComponent::StartDeath(const Entity& inThis)
{
	if (mIsDying)
	{
		return;
	}

	mIsDying		= true;
	mTimeUntilDeath	= MonsterDeathAnimation::sGetDuration();

	inThis.GetComponent<AnimationComponent>()->SetAnimation( MonsterDeathAnimation::sGetAnimation() );
}

void MonsterComponent::OnEntityCollidedWith(Entity inThis, Entity inCollidingEntity) const
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
