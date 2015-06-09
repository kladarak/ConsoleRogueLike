#include "World.h"

Entity* World::CreateEntity()
{
	mEntities.push_back( new Entity(this) );
	return mEntities.back();
}

void World::DestroyEntity(Entity* inEntity)
{
	for (auto iter = mEntities.begin(); iter != mEntities.end(); ++iter)
	{
		if ( (*iter) == inEntity )
		{
			mEntities.erase(iter);
			delete inEntity;
			break;
		}
	}
}
