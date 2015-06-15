#include "Bow.h"

#include <EntityComponent/Components/MessageReceiverComponent.h>
#include <EntityComponent/Components/PlayerComponent.h>
#include <EntityComponent/Components/PositionComponent.h>

#include <EntityComponent/Systems/PositionSystem.h>

#include <Messages/Messages.h>

#include "WeaponData.h"
#include "Arrow.h"
#include "BowAnimations.h"


Bow::Bow() 
	: Weapon(WeaponData::kBowData)
{
}

void Bow::Attack(Entity inPlayer, bool inStartedAttackThisFrame)
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
