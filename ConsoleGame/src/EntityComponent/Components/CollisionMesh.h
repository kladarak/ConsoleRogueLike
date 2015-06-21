#pragma once

#include <Containers/Dynamic2DVectorWithOffset.h>

class CollisionMesh : public Dynamic2DVectorWithOffset<bool>
{
public:
	CollisionMesh()								: Dynamic2DVectorWithOffset<bool>()									{ }
	CollisionMesh(CollisionMesh&& inRHS)		: Dynamic2DVectorWithOffset<bool>( inRHS )							{ }
	CollisionMesh(const CollisionMesh& inRHS)	: Dynamic2DVectorWithOffset<bool>( inRHS )							{ }
	CollisionMesh(int inX, int inY)				: Dynamic2DVectorWithOffset<bool>( 1, 1, true, IVec2(inX, inY) )	{ }
	CollisionMesh(const bool* inCollidableArray, size_t inCols, size_t inRows, const IVec2& inCentreOffset = IVec2(0, 0));

	void SetCollidableAt(int inX, int inY)		{ Set(inX, inY, true); }
	void ClearCollidableAt(int inX, int inY)	{ Set(inX, inY, false); }
	
	bool CollidesWith(const IVec2& inPos) const { return GetLocalBounds().Contains(inPos) && Get(inPos.mX, inPos.mY); }

	template<typename TFunctor>
	void ForEachCollidablePosition(const TFunctor& inFunctor) const;
};

template<typename TFunctor>
void CollisionMesh::ForEachCollidablePosition(const TFunctor& inFunctor) const
{
	ForEach( [&] (int inX, int inY, bool inCollidable)
	{
		if (inCollidable)
		{
			inFunctor(inX, inY);
		}
	} );
}
