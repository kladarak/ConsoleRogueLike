#include "WormMonster.h"

#include <EntityComponent/Components/MonsterComponent.h>
#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/ProgramComponent.h>

#include <EntityComponent/Systems/CollisionSystem.h>

#include <Containers/ContainerMacros.h>

#include <Messages/Messages.h>

#include "MonsterBuilder.h"

namespace WormMonster
{

static const float kMovementCooldownDuration = 1.0f;

namespace MonsterAnimation
{
	static const float kAnimationDuration = 0.5f;

	static const AsciiMesh kIdleFrames[] =
	{
		Fragment('e', ETextGreen),
		Fragment('@', ETextGreen),
	};

	static const Animation kIdle(kIdleFrames, gElemCount(kIdleFrames), kAnimationDuration, Animation::EPlaybackStyle_Loop);
}

class WormMonsterComponent
{
public:
	WormMonsterComponent();
	~WormMonsterComponent() { }
	
	void	Update(Entity inThis, float inFrameTime, MessageBroadcaster& inMsgBroadcaster);

private:
	IVec2	GetIntendedMovement(float inFrameTime);
	void	ResetMovementDuration();

	float	mMovementCooldownTime;
};

WormMonsterComponent::WormMonsterComponent()
{
	ResetMovementDuration();
}

void WormMonsterComponent::Update(Entity inThis, float inFrameTime, MessageBroadcaster&)
{
	if (inThis.GetComponent<MonsterComponent>()->IsDying())
	{
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

IVec2 WormMonsterComponent::GetIntendedMovement(float inFrameTime)
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

void WormMonsterComponent::ResetMovementDuration()
{
	mMovementCooldownTime = kMovementCooldownDuration + ((rand()%10) * 0.1f);
}

Entity Create(World& inWorld, MessageBroadcaster& inMsgBroadcaster, const IVec2& inPosition)
{
	auto entity = MonsterBuilder(inWorld, &inMsgBroadcaster)
					.SetAnimation( MonsterAnimation::kIdle )
					.SetRenderable( MonsterAnimation::kIdleFrames[0] )
					.SetPosition( inPosition )
					.Create();

	entity.AddComponent<WormMonsterComponent>();

	entity.GetComponent<ProgramComponent>()->RegisterProgram
	(
		[&inMsgBroadcaster] (const Entity& inThis, float inFrameTime)
		{
			inThis.GetComponent<WormMonsterComponent>()->Update(inThis, inFrameTime, inMsgBroadcaster); 
		} 
	);

	return entity;
}

}
