#pragma once

#include <vector>
#include "Entity.h"

class World
{
public:
	Entity*						CreateEntity();
	void						DestroyEntity(Entity* inEntity);
	
	const std::vector<Entity*>&	GetAllEntities() const { return mEntities; }

private:
	std::vector<Entity*>		mEntities;

};
 