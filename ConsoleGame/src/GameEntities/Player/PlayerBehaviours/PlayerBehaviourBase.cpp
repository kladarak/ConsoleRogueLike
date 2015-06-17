#include "PlayerBehaviourBase.h"

#include <EntityComponent/Components/AnimationComponent.h>
#include <EntityComponent/Components/CollisionComponent.h>
#include <EntityComponent/Components/MonsterComponent.h>
#include <EntityComponent/Components/PlayerComponent.h>
#include <EntityComponent/Components/PositionComponent.h>

#include <EntityComponent/Systems/CollisionSystem.h>

#include <GameEntities/Player/PlayerMeshes.h>

#include <Messages/Messages.h>

using namespace Player;

bool PlayerBehaviourBase::CanMoveToPosition(Entity inPlayer, const IVec2& inPosition) const
{
	auto& playerCollisionMesh = inPlayer.GetComponent<CollisionComponent>()->GetCollisionMesh();
	auto entitiesCollidedWith = CollisionSystem::GetListofCollidablesCollidedWith(*inPlayer.GetWorld(), playerCollisionMesh, inPosition);

	for (auto& collidable : entitiesCollidedWith)
	{
		if (collidable != inPlayer && !collidable.HasComponent<MonsterComponent>())
		{
			return false;
		}
	}

	return true;
}

bool PlayerBehaviourBase::OnAttacked(Entity inPlayer, const AttackMsg& inAttackMsg)
{
	// Move to random position away from direction of attack
	auto posComp = inPlayer.GetComponent<PositionComponent>();
	auto currPos = posComp->GetPosition();

	auto preferredPos = currPos + inAttackMsg.mAttackDirection;
	if ( CanMoveToPosition(inPlayer, preferredPos) )
	{
		posComp->SetPosition(preferredPos);
	}
	else
	{
		static const IVec2 kRecoveryPositions[] = { IVec2(0, 1), IVec2(0, -1), IVec2(1, 0), IVec2(-1, 0) };

		for (size_t i = 0; i < gElemCount(kRecoveryPositions); ++i)
		{
			IVec2 testPos = currPos + kRecoveryPositions[i];
			if ( !CollisionSystem::CollidesWithAnyEntity(*inPlayer.GetWorld(), inPlayer, testPos) )
			{
				posComp->SetPosition(testPos);
			}
		}
	}

	return true;
}

//--------------------------------------------------------------------

void PlayerIdleBehaviour::OnStart(Entity inPlayer)
{
	inPlayer.GetComponent<AnimationComponent>()->SetAnimations( GetIdleAnimations() );
	inPlayer.GetComponent<CollisionComponent>()->SetCollisionMesh( kIdleCollisionMesh );
	inPlayer.GetComponent<PlayerComponent>()->SetUsingItemSlot( EItemSlot_None );
}

void PlayerIdleBehaviour::OnRestart(Entity /*inPlayer*/)
{
}

void PlayerIdleBehaviour::Update(Entity /*inPlayer*/, float /*inFrameTime*/)
{
}

void PlayerIdleBehaviour::OnFinish(Entity /*inPlayer*/)
{
}

bool PlayerIdleBehaviour::IsFinished() const
{ 
	return false;
}
