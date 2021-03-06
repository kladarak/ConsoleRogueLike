#include "ProgramSystem.h"

#include <EntityComponentSystem/World/World.h>
#include <EntityComponentSystem/World/EntityFilter.h>

#include <EntityComponent/Components/ProgramComponent.h>

namespace ProgramSystem
{
	
static void UpdatePrograms(const Entity& inEntity, float inFrameTime)
{
	auto	programComp = inEntity.GetComponent<ProgramComponent>();
	auto&	programs	= programComp->GetPrograms();

	for (auto& program : programs)
	{
		if (inEntity.IsAlive()) // It may have died in a previous program
		{
			program(inEntity, inFrameTime);
		}
	}
}

void Update(World& inWorld, float inFrameTime)
{
	auto programEntities = inWorld.GetEntities( EntityFilter().MustHave<ProgramComponent>() );

	for (auto& entity : programEntities)
	{
		if (entity.IsAlive())  // It may have died in a previous entity's program
		{
			UpdatePrograms(entity, inFrameTime);
		}
	}
}


}
