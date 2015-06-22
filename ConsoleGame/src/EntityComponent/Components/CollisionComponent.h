#pragma once

#include "EOrientation.h"
#include "CollisionMesh.h"

class Entity;

class CollisionComponent
{
public:
	CollisionComponent();
	CollisionComponent(CollisionComponent&& inRHS);
	CollisionComponent(const CollisionComponent& inRHS);
	CollisionComponent(const CollisionMesh& inDefaultMesh);
	CollisionComponent(const CollisionMesh inMeshes[EOrientation_Count]);
	~CollisionComponent()	{ }
	
	void					SetDefaultCollisionMesh(const CollisionMesh& inMesh);
	const CollisionMesh&	GetDefaultCollisionMesh() const;
	
	void					SetCollisionMesh(EOrientation inOrientation, const CollisionMesh& inMesh);
	const CollisionMesh&	GetCollisionMesh(EOrientation inOrientation) const;
	
	void					SetCollisionMeshes(const CollisionMesh inMeshes[EOrientation_Count]);

	void					SetDefaultMeshCollidableAt(int inX, int inY);
	void					ClearDefaultMeshCollidableAt(int inX, int inY);

private:
	CollisionMesh			mCollisionMesh[EOrientation_Count];

};
