#pragma once

#include <Containers/Dynamic2DVectorWithOffset.h>

class Entity;

class CollisionComponent
{
public:
	CollisionComponent()	{ }
	~CollisionComponent()	{ }
	CollisionComponent(CollisionComponent&& inRHS)		: mCollidablePositions( std::move(inRHS.mCollidablePositions) )	{ }
	CollisionComponent(const CollisionComponent& inRHS) : mCollidablePositions( inRHS.mCollidablePositions )			{ }
	
	void							SetCollidableAt(int inX, int inY)			{ SetCollidableAt(IVec2(inX, inY)); }
	void							SetCollidableAt(const IVec2& inPosition);
	void							SetCollidableAt(const std::vector<IVec2>& inPositions);
	
	void							SetCentreOffset(const IVec2& inOffset)		{ mCollidablePositions.SetCentreOffset(inOffset);	}
	const IVec2&					GetCentreOffset() const						{ return mCollidablePositions.GetCentreOffset();	}

	template<typename TFunctor>
	void							ForEachCollidablePosition(const TFunctor& inFunctor) const;

	bool							CollidesWith(const IVec2& inPos) const;

private:
	Dynamic2DVectorWithOffset<bool> mCollidablePositions;

};

template<typename TFunctor>
void CollisionComponent::ForEachCollidablePosition(const TFunctor& inFunctor) const
{
	mCollidablePositions.ForEach( [&] (int inX, int inY, bool inCollidable)
	{
		if (inCollidable)
		{
			inFunctor(inX, inY);
		}
	} );
}