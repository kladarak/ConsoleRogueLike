#include "PlayerBehaviourBase.h"

#include <Containers/ContainerMacros.h>

#include <EntityComponent/Components/CollisionComponent.h>
#include <EntityComponent/Components/MonsterComponent.h>
#include <EntityComponent/Components/PositionComponent.h>

#include <EntityComponent/Systems/CollisionSystem.h>

#include <Messages/Messages.h>


bool PlayerBehaviourBase::CanMoveToPosition(Entity inPlayer, const IVec2& inPosition) const
{
	auto& playerCollisionMesh = inPlayer.GetComponent<CollisionComponent>()->GetDefaultCollisionMesh();
	auto entitiesCollidedWith = CollisionSystem::GetListofCollidablesCollidedWith(*inPlayer.GetWorld(), playerCollisionMesh, inPosition);

	for (auto& collidable : entitiesCollidedWith)
	{
		if (collidable == inPlayer)
		{
			// Ignore player
			continue;
		}
		
		// If it's not a monster, or the monster is passive when touched, then prevent movement into this space.
		auto monsterComponent = collidable.GetComponent<MonsterComponent>();
		if (nullptr == monsterComponent || monsterComponent->IsPassiveWhenTouched())
		{
			return false;
		}
	}

	return true;
}

bool PlayerBehaviourBase::OnAttacked(Entity inPlayer, const AttackMsg& inAttackMsg)
{
	if (inAttackMsg.mEffect == AttackMsg::EEffect_None)
	{
		// Do nothing; just indicate that player is injured.
		return true;
	}

	auto collidesWithAnything = [&] (const IVec2& inPosition)
	{
		return CollisionSystem::CollidesWithAnyEntity(*inPlayer.GetWorld(), inPlayer, inPosition);
	};

	auto posComp		= inPlayer.GetComponent<PositionComponent>();
	auto currPos		= posComp->GetPosition();
	auto lastPos		= posComp->GetPreviousPosition();
	auto pushedAwayPos	= currPos + inAttackMsg.mAttackDirection;

	if (pushedAwayPos != currPos && !collidesWithAnything(pushedAwayPos))
	{
		posComp->SetPosition(pushedAwayPos);
	}
	else if (lastPos != currPos && !collidesWithAnything(lastPos))
	{
		posComp->SetPosition(lastPos);
	}
	else
	{
		// Move to random position away from direction of attack
		static const IVec2 kRecoveryPositions[] = { IVec2(0, 1), IVec2(0, -1), IVec2(1, 0), IVec2(-1, 0) };

		for (size_t i = 0; i < gElemCount(kRecoveryPositions); ++i)
		{
			IVec2 testPos = currPos + kRecoveryPositions[i];
			if ( !collidesWithAnything(testPos) )
			{
				posComp->SetPosition(testPos);
				break;
			}
		}
	}

	return true;
}
