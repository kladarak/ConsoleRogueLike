#include "Boomerang.h"

#include <Containers/ContainerMacros.h>

#include <EntityComponent/Components/AnimationComponent.h>
#include <EntityComponent/Components/MonsterComponent.h>
#include <EntityComponent/Components/OrientationComponent.h>
#include <EntityComponent/Components/PlayerComponent.h>
#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/ProgramComponent.h>
#include <EntityComponent/Components/RenderableComponent.h>
#include <EntityComponent/Components/TriggerBoxComponent.h>

#include <EntityComponent/Systems/PositionSystem.h>

#include <EntityComponentSystem/World/EntityBuilder.h>

#include <Messages/Messages.h>

#include "ItemData.h"


static const AsciiMesh	kBoomerangHUDIcon( Fragment('<', ETextGreen) );
static const ItemData	kBoomerangData("Boomerang", kBoomerangHUDIcon, ERequiresNoAmmo );

static const AsciiMesh kKeyFrames[] =
{
	Fragment('^',					ETextGreen),
	Fragment(gCastUCharToChar(191), ETextGreen),
	Fragment('>',					ETextGreen),
	Fragment(gCastUCharToChar(217), ETextGreen),
	Fragment('v',					ETextGreen),
	Fragment(gCastUCharToChar(192), ETextGreen),
	Fragment('<',					ETextGreen),
	Fragment(gCastUCharToChar(218), ETextGreen),
};

static const Animation kAnimation(kKeyFrames, gElemCount(kKeyFrames), 0.1f, Animation::EPlaybackStyle_Loop);

static const int	kThrowDistance = 6;
static const float	kFlightTimeBetweenMovement = 0.1f;
static const float	kFlightTimeAwayFromPlayer = kFlightTimeBetweenMovement * kThrowDistance;

//------------------------------------------------------------------------------------

class BoomerangComponent
{
public:
	BoomerangComponent(BoomerangPlayerBehaviour* inPlayerBehaviour, const IVec2& inThrowDirection);
	BoomerangComponent(BoomerangComponent&& inRHS);
	~BoomerangComponent();

	void Update(Entity inThis, float inFrameTime, Entity inPlayer);
	
private:
	enum EState
	{
		EState_FlyAwayFromPlayer,
		EState_ReturnToPlayer
	};

	BoomerangPlayerBehaviour*	mBoomerangPlayerBehaviour;
	IVec2						mThrowDirection;
	EState						mState;
	float						mFlightTime;
	float						mTimeSinceMovement;
};

BoomerangComponent::BoomerangComponent(BoomerangPlayerBehaviour* inPlayerBehaviour, const IVec2& inThrowDirection)
	: mBoomerangPlayerBehaviour	(inPlayerBehaviour)
	, mThrowDirection			(inThrowDirection)
	, mState					(EState_FlyAwayFromPlayer)
	, mFlightTime				(0.0f)
	, mTimeSinceMovement		(0.0f)
{
}

BoomerangComponent::BoomerangComponent(BoomerangComponent&& inRHS)
	: mBoomerangPlayerBehaviour	(inRHS.mBoomerangPlayerBehaviour)
	, mThrowDirection			(inRHS.mThrowDirection)
	, mState					(inRHS.mState)
	, mFlightTime				(inRHS.mFlightTime)
	, mTimeSinceMovement		(inRHS.mTimeSinceMovement)
{
	inRHS.mBoomerangPlayerBehaviour = nullptr;
}

BoomerangComponent::~BoomerangComponent()
{
	if (nullptr != mBoomerangPlayerBehaviour)
	{
		mBoomerangPlayerBehaviour->OnBoomerangReturned();
	}
}

static bool sCollidesWithEnvironment(World& inWorld, const IVec2& inPosition)
{
	auto entitiesCollidedWith = CollisionSystem::GetListofCollidablesAtPosition(inWorld, inPosition);

	for (auto entity : entitiesCollidedWith)
	{
		if (!entity.HasComponent<PlayerComponent>() && !entity.HasComponent<MonsterComponent>())
		{
			return true;
		}
	}
	
	return false;
}

void BoomerangComponent::Update(Entity inThis, float inFrameTime, Entity inPlayer)
{
	mFlightTime			+= inFrameTime;
	mTimeSinceMovement	+= inFrameTime;

	IVec2 deltaPos(0, 0);

	if (mState == EState_FlyAwayFromPlayer)
	{
		if (mFlightTime >= kFlightTimeAwayFromPlayer)
		{
			mState = EState_ReturnToPlayer;
		}
		else if (mTimeSinceMovement >= kFlightTimeBetweenMovement)
		{
			mTimeSinceMovement -= kFlightTimeBetweenMovement;

			IVec2 thisPos	= inThis.GetComponent<PositionComponent>()->GetPosition();
			IVec2 newPos	= thisPos + mThrowDirection;

			if (sCollidesWithEnvironment(*inThis.GetWorld(), newPos))
			{
				mState = EState_ReturnToPlayer;
			}
			else
			{
				deltaPos = mThrowDirection;
			}
		}
	}

	if (mState == EState_ReturnToPlayer)
	{
		if (mTimeSinceMovement >= kFlightTimeBetweenMovement)
		{
			mTimeSinceMovement -= kFlightTimeBetweenMovement;
			
			IVec2	thisPos			= inThis.GetComponent<PositionComponent>()->GetPosition();
			IVec2	playerPos		= inPlayer.GetComponent<PositionComponent>()->GetPosition();
			IVec2	thisToPlayer	= playerPos - thisPos;
			
			deltaPos.mX = (thisToPlayer.mX != 0) ? thisToPlayer.mX/abs(thisToPlayer.mX) : 0;
			deltaPos.mY = (thisToPlayer.mY != 0) ? thisToPlayer.mY/abs(thisToPlayer.mY) : 0;

			IVec2 newPos = thisPos + deltaPos;

			// If player is at this position on return, then we've returned.
			if (newPos == playerPos || sCollidesWithEnvironment(*inThis.GetWorld(), newPos))
			{
				inThis.Kill();
			}
		}
	}

	if (inThis.IsAlive())
	{
		auto	posComp = inThis.GetComponent<PositionComponent>();
		IVec2	thisPos	= posComp->GetPosition();
		IVec2	newPos	= thisPos + deltaPos;
		posComp->SetPosition(newPos);

		AttackMsg attackMsg(inThis, newPos, IVec2(0, 0), AttackMsg::EEffect_None);
		MessageHelpers::BroadcastMessageToEntitiesAtPosition(*inThis.GetWorld(), inPlayer, newPos, attackMsg);
	}
}

//------------------------------------------------------------------------------------

Boomerang::Boomerang() 
	: ItemBase(kBoomerangData)
{
}

//------------------------------------------------------------------------------------

BoomerangPlayerBehaviour::BoomerangPlayerBehaviour()
	: mBoomerangIsInFlight(false)
{
}

void BoomerangPlayerBehaviour::OnStart(Entity inPlayer)
{
	if (mBoomerangIsInFlight)
	{
		return;
	}

	auto playerPos		= inPlayer.GetComponent<PositionComponent>()->GetPosition();
	auto orientation	= inPlayer.GetComponent<OrientationComponent>()->GetOrientation();
	
	IVec2 throwDir		= gGetOrientationVector(orientation);
	IVec2 startPos		= playerPos + throwDir;

	auto boomerangEntity = EntityBuilder(*inPlayer.GetWorld())
							.AddComponent<PositionComponent>(startPos)
							.AddComponent<BoomerangComponent>(this, throwDir)
							.AddComponent<ProgramComponent>()
							.AddComponent<AnimationComponent>(kAnimation)
							.AddComponent<RenderableComponent>(kKeyFrames[0])
							.Create();
	
	boomerangEntity.GetComponent<ProgramComponent>()->RegisterProgram( [inPlayer] (Entity inThis, float inFrameTime) 
	{
		inThis.GetComponent<BoomerangComponent>()->Update(inThis, inFrameTime, inPlayer);
	} ) ;

	mBoomerangIsInFlight = true;
}

void BoomerangPlayerBehaviour::OnRestart(Entity inPlayer)
{
	OnStart(inPlayer);
}

void BoomerangPlayerBehaviour::Update(Entity /*inPlayer*/, float /*inFrameTime*/)
{

}

void BoomerangPlayerBehaviour::OnFinish(Entity /*inPlayer*/)
{

}

bool BoomerangPlayerBehaviour::IsFinished() const
{ 
	return true;
}

void BoomerangPlayerBehaviour::OnBoomerangReturned()
{
	mBoomerangIsInFlight = false;
}
