#pragma once

#include "CollisionMesh.h"

class Entity;

class CollisionComponent
{
public:
	CollisionComponent()	{ }
	CollisionComponent(CollisionComponent&& inRHS)		: mCollisionMesh( std::move(inRHS.mCollisionMesh) )	{ }
	CollisionComponent(const CollisionComponent& inRHS) : mCollisionMesh( inRHS.mCollisionMesh )			{ }
	CollisionComponent(const CollisionMesh& inRHS)		: mCollisionMesh( inRHS )							{ }
	~CollisionComponent()	{ }
	
	void					SetCollisionMesh(const CollisionMesh& iMesh)		{ mCollisionMesh = iMesh; }
	const CollisionMesh&	GetCollisionMesh() const							{ return mCollisionMesh; }

	void					SetCollidableAt(int inX, int inY)							{ mCollisionMesh.Set(inX, inY, true); }
	
	void					SetCentreOffset(const IVec2& inOffset)						{ mCollisionMesh.SetCentreOffset(inOffset);	}
	const IVec2&			GetCentreOffset() const										{ return mCollisionMesh.GetCentreOffset();	}

	template<typename TFunctor>
	void					ForEachCollidablePosition(const TFunctor& inFunctor) const	{ mCollisionMesh.ForEachCollidablePosition( inFunctor ); }

	bool					CollidesWith(const IVec2& inPos) const						{ return mCollisionMesh.CollidesWith(inPos); }

private:
	CollisionMesh			mCollisionMesh;

};
