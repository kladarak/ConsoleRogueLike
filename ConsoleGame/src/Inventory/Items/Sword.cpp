#include "Sword.h"

#include <Containers/ContainerMacros.h>

#include <EntityComponent/Components/AnimationComponent.h>
#include <EntityComponent/Components/MessageReceiverComponent.h>
#include <EntityComponent/Components/OrientationComponent.h>
#include <EntityComponent/Components/PositionComponent.h>

#include <EntityComponent/Systems/PositionSystem.h>

#include <Messages/Messages.h>

#include "SwordAnimations.h"
#include "ItemData.h"

static const char		kSwordIcon[]	= {	'-','|','-','-','-' };
static const ItemData	kSwordData		= { "Sword", AsciiMesh(kSwordIcon, gElemCount(kSwordIcon), 1) };
static const float		kSwipeTime		= 0.5f;

using namespace Player;

Sword::Sword() 
	:	ItemBase(kSwordData)
{
}

void SwordPlayerBehaviour::OnStart(Entity inPlayer)
{
	inPlayer.GetComponent<AnimationComponent>()->SetAnimations( SwordAnimations::Generate() );
	mSwipeTimeElapsed = 0.0f;
}

void SwordPlayerBehaviour::OnRestart(Entity inPlayer)
{
	OnStart(inPlayer);
}

void SwordPlayerBehaviour::Update(Entity inPlayer, float inFrameTime)
{
	mSwipeTimeElapsed += inFrameTime;

	auto	playerPos	= inPlayer.GetComponent<PositionComponent>()->GetPosition();
	auto	orientation	= inPlayer.GetComponent<OrientationComponent>()->GetOrientation();
	IVec2	attackDir	= gGetOrientationVector(orientation);

	IVec2		attackPos = playerPos + attackDir;
	AttackMsg	attackMsg(inPlayer, attackPos, attackDir, AttackMsg::EEffect_None);
	MessageHelpers::BroadcastMessageToEntitiesAtPosition(*inPlayer.GetWorld(), inPlayer, attackPos, attackMsg);
}

void SwordPlayerBehaviour::OnFinish(Entity /*inPlayer*/)
{
}

bool SwordPlayerBehaviour::IsFinished() const
{ 
	return (mSwipeTimeElapsed > kSwipeTime);
}
