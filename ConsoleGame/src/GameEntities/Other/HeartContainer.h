#pragma once

#include <EntityComponentSystem/World/World.h>

class GameData;

namespace HeartContainer
{
	Entity Create(World& inWorld, GameData* inGameData);
}
