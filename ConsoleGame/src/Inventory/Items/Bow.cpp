#include "Bow.h"

#include <Containers/ContainerMacros.h>

#include <EntityComponent/Components/AnimationComponent.h>
#include <EntityComponent/Components/OrientationComponent.h>
#include <EntityComponent/Components/PlayerComponent.h>
#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/RenderableComponent.h>
#include <EntityComponent/Components/TriggerBoxComponent.h>

#include <EntityComponent/Systems/PositionSystem.h>

#include <EntityComponentSystem/World/EntityBuilder.h>

#include <Messages/Messages.h>

#include "ItemData.h"
#include "Arrow.h"
#include "BowAnimations.h"


static const Fragment	kBowIcon[]	= { Fragment('D', ETextDarkYellow), ' ', Fragment(26, ETextYellow) };
static const ItemData	kBowData("Bow", AsciiMesh(kBowIcon, gElemCount(kBowIcon), 1), ERequiresAmmo );
static const float		kAnimTime	= 0.5f;

using namespace Player;

Bow::Bow() 
	: ItemBase(kBowData)
{
}

void Bow::SpawnAmmo(World& inWorld, const IVec2& inPosition)
{
	auto arrow = EntityBuilder(inWorld)
					.AddComponent<PositionComponent>(inPosition)
					.AddComponent<RenderableComponent>( Fragment(24, ETextYellow) )
					.AddComponent<TriggerBoxComponent>()
					.Create();

	arrow.GetComponent<TriggerBoxComponent>()->RegisterOnEnterCallback
	( 
		[&] (Entity inThis, Entity inPlayer)
		{
			if (inPlayer.HasComponent<PlayerComponent>())
			{
				mBehaviour.IncArrowCount();
				inThis.Kill();
			}
		}
	);
}

//------------------------------------------------------------------------------------

BowPlayerBehaviour::BowPlayerBehaviour() 
	: mAnimTimeElapsed(kAnimTime)
	, mArrowCount(10)
{
}

void BowPlayerBehaviour::OnStart(Entity inPlayer)
{
	if (mArrowCount == 0)
	{
		return;
	}

	inPlayer.GetComponent<AnimationComponent>()->SetAnimations( BowAnimations::Generate() );

	auto playerPos		= inPlayer.GetComponent<PositionComponent>()->GetPosition();
	auto orientation	= inPlayer.GetComponent<OrientationComponent>()->GetOrientation();
	
	IVec2 attackDir		= gGetOrientationVector(orientation);
	IVec2 attackPos		= playerPos + attackDir;
	
	Arrow::Create(*inPlayer.GetWorld(), attackPos, attackDir);
	--mArrowCount;

	mAnimTimeElapsed = 0.0f;
}

void BowPlayerBehaviour::OnRestart(Entity inPlayer)
{
	if (IsFinished())
	{
		OnStart(inPlayer);
	}
}

void BowPlayerBehaviour::Update(Entity /*inPlayer*/, float inFrameTime)
{
	mAnimTimeElapsed += inFrameTime;
}

void BowPlayerBehaviour::OnFinish(Entity /*inPlayer*/)
{
	mAnimTimeElapsed = kAnimTime;
}

bool BowPlayerBehaviour::IsFinished() const
{ 
	return mAnimTimeElapsed >= kAnimTime;
}
