#include "Sword.h"

#include <EntityComponent/Components/MessageReceiverComponent.h>
#include <EntityComponent/Components/PlayerComponent.h>
#include <EntityComponent/Components/PositionComponent.h>

#include <EntityComponent/Systems/PositionSystem.h>

#include <Messages/Messages.h>

#include "SwordAnimations.h"

void Sword::Attack(Entity inPlayer)
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
		
	auto attackedEntities = PositionSystem::GetListOfEntitiesAtPosition(*inPlayer.GetWorld(), inPlayer, attackPos);
	for (auto entity : attackedEntities)
	{
		auto msgRecComp = entity.GetComponent<MessageReceiverComponent>();
		if (nullptr != msgRecComp)
		{
			msgRecComp->Broadcast( AttackMsg(inPlayer, attackPos, attackDir) );
		}
	}
}

std::vector<Animation> Sword::GetAnimations() const
{
	return SwordAnimations::Generate();
}
