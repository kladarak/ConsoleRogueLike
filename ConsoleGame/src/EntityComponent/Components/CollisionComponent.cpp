#include "CollisionComponent.h"

void CollisionComponent::SetCollidableAt(const IVec2& inPosition)
{
	mCollidablePositions.push_back(inPosition);
}

void CollisionComponent::SetCollidableAt(const std::vector<IVec2>& inPositions)
{
	for (auto& pos : inPositions)
	{
		SetCollidableAt(pos);
	}
}
