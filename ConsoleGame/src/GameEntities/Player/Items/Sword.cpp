#include "Sword.h"

#include <Containers/ContainerMacros.h>

#include <EntityComponent/Components/AnimationComponent.h>
#include <EntityComponent/Components/MessageReceiverComponent.h>
#include <EntityComponent/Components/PlayerComponent.h>
#include <EntityComponent/Components/PositionComponent.h>

#include <EntityComponent/Systems/PositionSystem.h>

#include <Messages/Messages.h>

#include "SwordAnimations.h"
#include "ItemData.h"

static const char kSwordIcon[]		= {	'-','|','-','-','-' };
static const ItemData kSwordData	= { "Sword", AsciiMesh(kSwordIcon, gElemCount(kSwordIcon), 1) };

Sword::Sword() 
	: ItemBase(kSwordData)
{
}

void Sword::Use(Entity inPlayer, bool inStartedAttackThisFrame)
{
	using namespace Player;

	auto playerPos			= inPlayer.GetComponent<PositionComponent>()->GetPosition();
	auto facingDirection	= inPlayer.GetComponent<PlayerComponent>()->GetFacingDirection();

	IVec2 attackDir(0, 0);
	switch (facingDirection)
	{
		case EFacingDirection_Left:		attackDir.mX = -1; break;
		case EFacingDirection_Right:	attackDir.mX =  1; break;
		case EFacingDirection_Up:		attackDir.mY = -1; break;
		case EFacingDirection_Down:		attackDir.mY =  1; break;
	}

	IVec2 attackPos = playerPos + attackDir;
		
	AttackMsg attackMsg(inPlayer, attackPos, attackDir);
	MessageHelpers::BroadcastMessageToEntitiesAtPosition(*inPlayer.GetWorld(), inPlayer, attackPos, attackMsg);

	if (inStartedAttackThisFrame)
	{
		inPlayer.GetComponent<AnimationComponent>()->ResetSelectedAnimation();
	}
}

std::vector<Animation> Sword::GetAnimations() const
{
	return SwordAnimations::Generate();
}
