#include "PlayerBehaviourBase.h"

#include <EntityComponentSystem/World/World.h>

#include <EntityComponent/Components/CollisionComponent.h>
#include <EntityComponent/Components/MonsterComponent.h>

#include <EntityComponent/Systems/CollisionSystem.h>

void PlayerBehaviourBase::ApplyComponentModifiers(Entity /*inPlayer*/)
{
}

void PlayerBehaviourBase::Update(Entity /*inPlayer*/, float /*inFrameTime*/)
{
}

bool PlayerBehaviourBase::CanMoveToPosition(Entity inPlayer, const IVec2& inPosition)
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