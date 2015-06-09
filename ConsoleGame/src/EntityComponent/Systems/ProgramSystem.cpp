#include "ProgramSystem.h"

#include <EntityComponentSystem/World/World.h>

#include <EntityComponent/Components/ProgramComponent.h>

namespace ProgramSystem
{
	
static void UpdatePrograms(const Entity& inEntity, float inFrameTime)
{
	auto programComp = inEntity.GetComponent<ProgramComponent>();
	if (nullptr == programComp)
	{
		return;
	}

	auto& programs = programComp->GetPrograms();
	for (auto& program : programs)
	{
		program(inEntity, inFrameTime);
	}
}

void Update(World* inWorld, float inFrameTime)
{
	inWorld->ForEachEntity( [&] (Entity inEntity)
	{
		UpdatePrograms(inEntity, inFrameTime);
	} );
}


}
