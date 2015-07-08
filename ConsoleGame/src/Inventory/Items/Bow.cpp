#include "Bow.h"

#include <Containers/ContainerMacros.h>

#include <EntityComponent/Components/AnimationComponent.h>
#include <EntityComponent/Components/OrientationComponent.h>
#include <EntityComponent/Components/PositionComponent.h>

#include <EntityComponent/Systems/PositionSystem.h>

#include <Messages/Messages.h>

#include "ItemData.h"
#include "Arrow.h"
#include "BowAnimations.h"


static const char		kBowIcon[]	= { 'D', ' ', 26 };
static const ItemData	kBowData	= { "Bow", AsciiMesh(kBowIcon, gElemCount(kBowIcon), 1) };
static const float		kAnimTime	= 0.5f;

using namespace Player;

Bow::Bow() 
	: ItemBase(kBowData)
{
}

BowPlayerBehaviour::BowPlayerBehaviour() 
	: mAnimTimeElapsed(kAnimTime)
	, mArrowCount(10)
{
}

void BowPlayerBehaviour::OnStart(Entity inPlayer)
{
	inPlayer.GetComponent<AnimationComponent>()->SetAnimations( BowAnimations::Generate() );

	auto playerPos		= inPlayer.GetComponent<PositionComponent>()->GetPosition();
	auto orientation	= inPlayer.GetComponent<OrientationComponent>()->GetOrientation();
	
	IVec2 attackDir		= gGetOrientationVector(orientation);
	IVec2 attackPos		= playerPos + attackDir;
	
	if (mArrowCount > 0)
	{
		Arrow::Create(*inPlayer.GetWorld(), attackPos, attackDir);
		--mArrowCount;
	}

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
