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
		program(inEntity, inFrameTime);
	}
}

void Update(World& inWorld, float inFrameTime)
{
	auto programEntities = inWorld.GetEntities( EntityFilter().MustHave<ProgramComponent>() );

	for (auto& entity : programEntities)
	{
		UpdatePrograms(entity, inFrameTime);
	}
}


}
