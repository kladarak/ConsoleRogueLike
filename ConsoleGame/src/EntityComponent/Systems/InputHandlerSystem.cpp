#include "InputHandlerSystem.h"

#include <Input/InputBuffer.h>

#include <EntityComponentSystem/World/World.h>

#include <EntityComponent/Components/InputHandlerComponent.h>


namespace InputHandlerSystem
{

static void HandleInput(const Entity& inEntity, const InputBuffer& inInputBuffer)
{
	auto inputHandlerComp = inEntity.GetComponent<InputHandlerComponent>();
	if (nullptr == inputHandlerComp)
	{
		return;
	}

	auto& handlers = inputHandlerComp->GetHandlers();
	for (auto& handler : handlers)
	{
		handler(inEntity, inInputBuffer);
	}
}

void Update(World* inWorld, const InputBuffer& inInputBuffer)
{
	inWorld->ForEachEntity( [&] (Entity inEntity)
	{
		HandleInput(inEntity, inInputBuffer);
	} );
}

}
