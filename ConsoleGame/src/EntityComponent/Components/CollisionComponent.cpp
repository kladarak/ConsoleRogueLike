#include "CollisionComponent.h"


CollisionComponent::CollisionComponent()
{
}

CollisionComponent::CollisionComponent(CollisionComponent&& inRHS)
{
	for (int i = 0; i < EOrientation_Count; ++i)
	{
		mCollisionMesh[i] = std::move( inRHS.mCollisionMesh[i] );
	}
}

CollisionComponent::CollisionComponent(const CollisionComponent& inRHS)
{
	SetCollisionMeshes(inRHS.mCollisionMesh);
}

CollisionComponent::CollisionComponent(const CollisionMesh& inDefaultMesh)
{
	SetDefaultCollisionMesh(inDefaultMesh);
}

CollisionComponent::CollisionComponent(const CollisionMesh inMeshes[EOrientation_Count])
{
	SetCollisionMeshes(inMeshes);
}

void CollisionComponent::SetDefaultCollisionMesh(const CollisionMesh& inMesh)
{
	for (int i = 0; i < EOrientation_Count; ++i)
	{
		SetCollisionMesh( (EOrientation) i, inMesh );
	}
}

const CollisionMesh& CollisionComponent::GetDefaultCollisionMesh() const
{
	return GetCollisionMesh( EOrientation_FaceUp );
}
	
void CollisionComponent::SetCollisionMesh(EOrientation inOrientation, const CollisionMesh& inMesh)
{
	assert(inOrientation != EOrientation_Count);
	mCollisionMesh[inOrientation] = inMesh;
}

const CollisionMesh& CollisionComponent::GetCollisionMesh(EOrientation inOrientation) const
{
	assert(inOrientation != EOrientation_Count);
	return mCollisionMesh[inOrientation];
}

void CollisionComponent::SetCollisionMeshes(const CollisionMesh inMeshes[EOrientation_Count])
{
	for (int i = 0; i < EOrientation_Count; ++i)
	{
		SetCollisionMesh( (EOrientation) i, inMeshes[i] );
	}
}

void CollisionComponent::SetDefaultMeshCollidableAt(int inX, int inY)
{
	for (int i = 0; i < EOrientation_Count; ++i)
	{
		mCollisionMesh[i].SetCollidableAt(inX, inY);
	}
}

void CollisionComponent::ClearDefaultMeshCollidableAt(int inX, int inY)
{
	for (int i = 0; i < EOrientation_Count; ++i)
	{
		mCollisionMesh[i].ClearCollidableAt(inX, inY);
	}
}
