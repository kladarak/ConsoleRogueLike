#include "CollisionComponent.h"

void CollisionComponent::SetCollidableAt(const IVec2& inPosition)
{
	mCollidablePositions.Set(inPosition.mX, inPosition.mY, true);
}

void CollisionComponent::SetCollidableAt(const std::vector<IVec2>& inPositions)
{
	for (auto& pos : inPositions)
	{
		SetCollidableAt(pos);
	}
}

bool CollisionComponent::CollidesWith(const IVec2& inPos) const
{
	auto bounds = mCollidablePositions.GetLocalBounds();
	
	return bounds.Contains(inPos) && mCollidablePositions.Get(inPos.mX, inPos.mY);
}
