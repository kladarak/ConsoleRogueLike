#pragma once

#include <vector>
#include <Maths/IVec2.h>

class Entity;

class CollisionComponent
{
public:
	CollisionComponent()	{ }
	~CollisionComponent()	{ }
	CollisionComponent(CollisionComponent&& inRHS)		: mCollidablePositions( std::move(inRHS.mCollidablePositions) )	{ }
	CollisionComponent(const CollisionComponent& inRHS) : mCollidablePositions( inRHS.mCollidablePositions )			{ }
	
	void						SetCollidableAt(int inX, int inY)			{ SetCollidableAt(IVec2(inX, inY)); }
	void						SetCollidableAt(const IVec2& inPosition);
	void						SetCollidableAt(const std::vector<IVec2>& inPositions);

	const std::vector<IVec2>&	GetCollidablePositions() const				{ return mCollidablePositions; }

private:
	std::vector<IVec2>			mCollidablePositions;
};
