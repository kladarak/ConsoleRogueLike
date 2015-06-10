#include "InputHandlerSystem.h"

#include <Input/InputBuffer.h>

#include <EntityComponentSystem/World/World.h>
#include <EntityComponentSystem/World/EntityFilter.h>

#include <EntityComponent/Components/InputHandlerComponent.h>


namespace InputHandlerSystem
{

static void HandleInput(const Entity& inEntity, const InputBuffer& inInputBuffer)
{
	auto	inputHandlerComp	= inEntity.GetComponent<InputHandlerComponent>();
	auto&	handlers			= inputHandlerComp->GetHandlers();

	for (auto& handler : handlers)
	{
		handler(inEntity, inInputBuffer);
	}
}

void Update(World& inWorld, const InputBuffer& inInputBuffer)
{
	auto inputHandlingEntities = inWorld.GetEntities( EntityFilter().MustHave<InputHandlerComponent>() );

	for (auto& entity : inputHandlingEntities)
	{
		HandleInput(entity, inInputBuffer);
	}
}

}
