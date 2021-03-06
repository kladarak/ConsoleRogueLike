#pragma once

#include <EntityComponentSystem/World/World.h>
#include <EntityComponentSystem/World/EntityBuilder.h>

#include <EntityComponent/Components/PlayerComponent.h>
#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/RenderableComponent.h>
#include <EntityComponent/Components/TriggerBoxComponent.h>

#include "GameData.h"

namespace ItemEntity
{
	template<typename TItemBase>
	Entity Create(World& inWorld, GameData* inGameData)
	{
		auto mesh	= TItemBase().GetHUDIcon();
		auto offset = mesh.GetCentreOffset();
		
		IRect triggerBox;
		triggerBox.mX = offset.mX;
		triggerBox.mY = offset.mY;
		triggerBox.mWidth = mesh.GetColCount();
		triggerBox.mHeight = mesh.GetRowCount();

		auto entity = EntityBuilder(inWorld)
						.AddComponent<PositionComponent>(0, 0)
						.AddComponent<RenderableComponent>(mesh)
						.AddComponent<TriggerBoxComponent>( triggerBox )
						.Create();
	
		entity.GetComponent<TriggerBoxComponent>()->RegisterOnEnterCallback
		(
			[=] (Entity inItemEntity, Entity inPlayer)
			{
				if (inPlayer.HasComponent<PlayerComponent>())
				{
					using namespace Player;

					auto& playerData = inGameData->mPlayerData;

					auto item = new TItemBase();
					playerData.mInventory.AddItem( item );

					EItemSlot emptySlot = (playerData.GetItemInSlot(EItemSlot_Slot0) == nullptr) ? EItemSlot_Slot0
											: ((playerData.GetItemInSlot(EItemSlot_Slot1) == nullptr) ? EItemSlot_Slot1 
												: EItemSlot_None);

					if (emptySlot != EItemSlot_None)
					{
						inGameData->mPlayerData.SetItemInSlot(item, emptySlot);
					}

					inItemEntity.Kill();
				}
			}
		);
	
		return entity;
	}

}
