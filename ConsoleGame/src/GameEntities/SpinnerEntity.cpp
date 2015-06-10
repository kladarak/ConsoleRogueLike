#include "SpinnerEntity.h"

#include <EntityComponentSystem/World/World.h>

#include <EntityComponent/Components/CollisionComponent.h>
#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/ProgramComponent.h>
#include <EntityComponent/Components/RenderableComponent.h>

namespace SpinnerEntity
{

static const char*	kSpinnerStates = "|/-\\";
static const float	kStateChangeRate = 0.5f; // seconds

class SpinnerState
{
public:
	SpinnerState() : mCurrentState(0), mElapsedTime(0.0f) { }
	~SpinnerState()	{ }
	SpinnerState(SpinnerState&& inRHS)		: mCurrentState(inRHS.mCurrentState), mElapsedTime(inRHS.mElapsedTime) { }
	SpinnerState(const SpinnerState& inRHS)	: mCurrentState(inRHS.mCurrentState), mElapsedTime(inRHS.mElapsedTime) { }

	int		mCurrentState;
	float	mElapsedTime;
};

static void Update(const Entity& inThis, float inFrameTime)
{
	auto& spinnerState = *inThis.GetComponent<SpinnerState>();

	spinnerState.mElapsedTime += inFrameTime;

	if (spinnerState.mElapsedTime >= kStateChangeRate)
	{
		spinnerState.mElapsedTime -= kStateChangeRate;

		int currentState			= spinnerState.mCurrentState;
		currentState				= (currentState + 1) % sizeof(kSpinnerStates);
		spinnerState.mCurrentState	= currentState;

		AsciiMesh mesh;
		mesh.SetCharAtPosition(0, 0, kSpinnerStates[currentState]);
		inThis.GetComponent<RenderableComponent>()->SetMesh(mesh);
	}
}

void Create(World& inWorld, const IVec2& inPos)
{
	Entity entity = inWorld.CreateEntity();

	auto positionComp		= entity.AddComponent<PositionComponent>();
	auto programHandlerComp	= entity.AddComponent<ProgramComponent>();
	auto collisionComp		= entity.AddComponent<CollisionComponent>();
	auto renderableComp		= entity.AddComponent<RenderableComponent>();
	entity.AddComponent<SpinnerState>();

	positionComp->SetPosition( inPos );
	programHandlerComp->RegisterProgram( &Update );
	collisionComp->SetCollidableAt( IVec2(0, 0) );
	
	AsciiMesh mesh;
	mesh.SetCharAtPosition(0, 0, kSpinnerStates[0]);
	renderableComp->SetMesh(mesh);
}

}
