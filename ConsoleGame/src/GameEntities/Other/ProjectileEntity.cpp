#include "ProjectileEntity.h"

#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/ProgramComponent.h>
#include <EntityComponent/Components/RenderableComponent.h>
#include <EntityComponent/Components/TriggerBoxComponent.h>

#include <EntityComponent/Systems/CollisionSystem.h>

#include <Messages/Messages.h>


namespace ProjectileEntity
{

static const float kMovementTime = 0.05f;

class ProjectileUpdateState
{
public:
	ProjectileUpdateState(const IVec2& inDirection) 
		: mTimeUntilMovementStep(kMovementTime)
		, mDirection(inDirection)
	{ 
	}

	~ProjectileUpdateState() { }

	IVec2 mDirection;
	float mTimeUntilMovementStep;
};

static void Update(Entity inThis, float inFrameTime)
{
	auto state		= inThis.GetComponent<ProjectileUpdateState>();
	auto posComp	= inThis.GetComponent<PositionComponent>();

	state->mTimeUntilMovementStep -= inFrameTime;
			
	if (state->mTimeUntilMovementStep <= 0.0f)
	{
		state->mTimeUntilMovementStep += kMovementTime;

		auto position = posComp->GetPosition();
		position += state->mDirection;
		posComp->SetPosition(position);
		
		AttackMsg attackMsg(inThis, position, state->mDirection, AttackMsg::EEffect_None);
		MessageHelpers::BroadcastMessageToEntitiesAtPosition(*inThis.GetWorld(), inThis, position, attackMsg);

		bool collides = CollisionSystem::CollidesWithAnyEntity(*inThis.GetWorld(), inThis, position);
		if (collides)
		{
			inThis.Kill();
		}
	}
}

Entity Create(World& inWorld, const AsciiMesh& inMesh, const IVec2& inPosition, const IVec2& inDirection)
{
	auto entity = inWorld.CreateEntity();

	entity.AddComponent<ProjectileUpdateState>(inDirection);
	entity.AddComponent<PositionComponent>(inPosition);
	entity.AddComponent<ProgramComponent>()->RegisterProgram( &Update );
	entity.AddComponent<RenderableComponent>( inMesh );

	entity.AddComponent<TriggerBoxComponent>()->RegisterOnEnterCallback( 
		[] (Entity inArrow, const Entity& inTriggerer)
		{
			auto msgRecComp = inTriggerer.GetComponent<MessageReceiverComponent>();
			if (nullptr != msgRecComp)
			{
				auto position = inArrow.GetComponent<PositionComponent>()->GetPosition();
				auto direction = inArrow.GetComponent<ProjectileUpdateState>()->mDirection;
				msgRecComp->Broadcast( AttackMsg(inArrow, position, direction, AttackMsg::EEffect_None) );
			}

			inArrow.Kill();
		} );

	return entity;
}

}
