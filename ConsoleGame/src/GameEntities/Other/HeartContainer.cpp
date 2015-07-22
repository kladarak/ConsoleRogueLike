#include "HeartContainer.h"

#include <EntityComponentSystem/World/EntityBuilder.h>

#include <EntityComponent/Components/HealthComponent.h>
#include <EntityComponent/Components/PlayerComponent.h>
#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/RenderableComponent.h>
#include <EntityComponent/Components/TriggerBoxComponent.h>

#include "GameData.h"

namespace HeartContainer
{

// Template:
// .'`'.'`'.
// `.     .`
//   `. .`
//     '

// After editing:
// .:::.:::.
// `:::::::`
//   `:::`
//     '

static const Fragment kFrags[] =
{
	Fragment('.', ETextRed),	Fragment(':', ETextRed),	Fragment(':', ETextRed),	Fragment(':', ETextRed),	Fragment('.', ETextRed),	Fragment(':', ETextRed),	Fragment(':', ETextRed),	Fragment(':', ETextRed),	Fragment('.', ETextRed),
	Fragment(':', ETextRed),	Fragment(':', ETextRed),	Fragment(':', ETextRed),	Fragment(':', ETextRed),	Fragment(':', ETextRed),	Fragment(':', ETextRed),	Fragment(':', ETextRed),	Fragment(':', ETextRed),	Fragment(':', ETextRed),
	Fragment('`', ETextRed),	Fragment(':', ETextRed),	Fragment(':', ETextRed),	Fragment(':', ETextRed),	Fragment(':', ETextRed),	Fragment(':', ETextRed),	Fragment(':', ETextRed),	Fragment(':', ETextRed),	Fragment('`', ETextRed),
	Fragment(' ', EInvisible),	Fragment(' ', EInvisible),	Fragment('`', ETextRed),	Fragment(':', ETextRed),	Fragment(':', ETextRed),	Fragment(':', ETextRed),	Fragment('`', ETextRed),	Fragment(' ', EInvisible),	Fragment(' ', EInvisible),
	Fragment(' ', EInvisible),	Fragment(' ', EInvisible),	Fragment(' ', EInvisible),	Fragment(' ', EInvisible),	Fragment('\'', ETextRed),	Fragment(' ', EInvisible),	Fragment(' ', EInvisible),	Fragment(' ', EInvisible),	Fragment(' ', EInvisible),
};
static const AsciiMesh kRenderMesh(kFrags, 9, 5);


Entity Create(World& inWorld, GameData* inGameData)
{
	auto entity = EntityBuilder(inWorld)
					.AddComponent<PositionComponent>(0, 0)
					.AddComponent<RenderableComponent>(kRenderMesh)
					.AddComponent<TriggerBoxComponent>( IRect(2, 0, 5, 4) )
					.Create();

	entity.GetComponent<TriggerBoxComponent>()->RegisterOnEnterCallback( [=] (Entity inThis, Entity inTriggerer)
	{
		if (inTriggerer.HasComponent<PlayerComponent>())
		{
			auto& maxHealth = inGameData->mPlayerData.mMaxHealth;
			auto healthComp = inGameData->mPlayer.GetComponent<HealthComponent>();

			maxHealth++;
			healthComp->SetMaxHealth(maxHealth);
			healthComp->SetCurrentHealth(maxHealth);

			inThis.Kill();
		}
	} );

	return entity;
}

}
