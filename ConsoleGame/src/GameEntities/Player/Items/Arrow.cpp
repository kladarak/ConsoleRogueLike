#include "Arrow.h"

#include <EntityComponentSystem/World/World.h>

#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/ProgramComponent.h>
#include <EntityComponent/Components/RenderableComponent.h>
#include <EntityComponent/Components/TriggerBoxComponent.h>

#include <EntityComponent/Systems/CollisionSystem.h>

#include <Messages/Messages.h>

namespace Arrow
{

static const float kMovementTime = 0.05f;
static const AsciiMesh kMeshes[] = 
{
	(char) 24, 
	(char) 25, 
	(char) 26, 
	(char) 27
};

class ArrowUpdateState
{
public:
	ArrowUpdateState(const IVec2& inDirection) 
		: mTimeUntilMovementStep(kMovementTime)
		, mDirection(inDirection)
	{ 
	}

	~ArrowUpdateState() { }

	IVec2 mDirection;
	float mTimeUntilMovementStep;
};

static void Update(Entity inThis, float inFrameTime)
{
	auto state		= inThis.GetComponent<ArrowUpdateState>();
	auto posComp	= inThis.GetComponent<PositionComponent>();

	state->mTimeUntilMovementStep -= inFrameTime;
			
	if (state->mTimeUntilMovementStep <= 0.0f)
	{
		state->mTimeUntilMovementStep += kMovementTime;

		auto position = posComp->GetPosition();
		position += state->mDirection;
		posComp->SetPosition(position);
		
		AttackMsg attackMsg(inThis, position, state->mDirection);
		MessageHelpers::BroadcastMessageToEntitiesAtPosition(*inThis.GetWorld(), inThis, position, attackMsg);

		bool collides = CollisionSystem::CollidesWithAnyEntity(*inThis.GetWorld(), inThis, position);
		if (collides)
		{
			inThis.Kill();
		}
	}
}

void Create(World& inWorld, const IVec2& inPosition, const IVec2& inDirection)
{
	int meshIndex = 0;

	if		(inDirection.mY < 0) { meshIndex = 0; }
	else if (inDirection.mY > 0) { meshIndex = 1; }
	else if	(inDirection.mX > 0) { meshIndex = 2; }
	else if (inDirection.mX < 0) { meshIndex = 3; }

	auto entity = inWorld.CreateEntity();

	entity.AddComponent<ArrowUpdateState>(inDirection);
	entity.AddComponent<PositionComponent>(inPosition);
	entity.AddComponent<ProgramComponent>()->RegisterProgram( &Update );
	entity.AddComponent<RenderableComponent>( kMeshes[meshIndex] );

	entity.AddComponent<TriggerBoxComponent>( IRect(0, 0, 1, 1) )->RegisterOnEnterCallback( 
		[] (Entity inArrow, const Entity& inTriggerer)
		{
			auto msgRecComp = inTriggerer.GetComponent<MessageReceiverComponent>();
			if (nullptr != msgRecComp)
			{
				auto position = inArrow.GetComponent<PositionComponent>()->GetPosition();
				auto direction = inArrow.GetComponent<ArrowUpdateState>()->mDirection;
				msgRecComp->Broadcast( AttackMsg(inArrow, position, direction) );
			}

			inArrow.Kill();
		} );
}

}
