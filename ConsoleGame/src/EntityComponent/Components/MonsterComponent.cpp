#include "MonsterComponent.h"

#include <Core/Messaging/MessageBroadcaster.h>

#include <EntityComponentSystem/World/World.h>

#include <EntityComponent/Components/AnimationComponent.h>
#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/MessageReceiverComponent.h>
#include <EntityComponent/Components/RenderableComponent.h>

#include <GameEntities/Monsters/MonsterDeathAnimation.h>

#include <Messages/Messages.h>

static const float kTotalDamageFlashTime	= 2.0f;
static const float kDamageFlashRate			= 10.0f;

MonsterComponent::MonsterComponent()
	: mDamageAnimationTime	(0.0f)
	, mTimeUntilDeath		(FLT_MAX)
	, mIsDying				(false)
{
}

void MonsterComponent::UpdateDamageAnimations(Entity inThis, float inFrameTime, MessageBroadcaster& inMsgBroadcaster)
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
	else if (mDamageAnimationTime > 0.0f)
	{
		mDamageAnimationTime -= inFrameTime;
		
		int		flashState	= (int) (mDamageAnimationTime * kDamageFlashRate);
		bool	visible		= (flashState % 2) == 0;

		inThis.GetComponent<RenderableComponent>()->SetVisible(visible);
	}
	else
	{
		inThis.GetComponent<RenderableComponent>()->SetVisible(true);
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
	inThis.GetComponent<RenderableComponent>()->SetVisible(true);
}

void MonsterComponent::StartDamagedAnimation()
{
	mDamageAnimationTime = kTotalDamageFlashTime;
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
		AttackMsg attackMsg(inThis, position, IVec2(0, 0), AttackMsg::EEffect_PushBack, 1);
		msgRecComp->Broadcast( attackMsg );
	}
}
