#pragma once

class World;

namespace TriggerSystem
{
	void Update(World& inWorld);
	void HandleDestroyedEntities(World& inWorld);
}
