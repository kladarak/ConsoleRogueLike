#include "Bow.h"

#include <Containers/ContainerMacros.h>

#include <EntityComponent/Components/MessageReceiverComponent.h>
#include <EntityComponent/Components/PlayerComponent.h>
#include <EntityComponent/Components/PositionComponent.h>

#include <EntityComponent/Systems/PositionSystem.h>

#include <Messages/Messages.h>

#include "ItemData.h"
#include "Arrow.h"
#include "BowAnimations.h"


static const char		kBowIcon[]	= { 'D', ' ', 26 };
static const ItemData	kBowData	= { "Bow", AsciiMesh(kBowIcon, gElemCount(kBowIcon), 1) };

Bow::Bow() 
	: ItemBase(kBowData)
{
}

void Bow::Use(Entity inPlayer, bool inStartedAttackThisFrame)
{
	if (!inStartedAttackThisFrame)
	{
		return;
	}

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
	
	Arrow::Create(*inPlayer.GetWorld(), attackPos, attackDir);
}

std::vector<Animation> Bow::GetAnimations() const
{
	return BowAnimations::Generate();
}
