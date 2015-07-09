#include "ItemDropHandler.h"

#include <Core/Messaging/MessageBroadcaster.h>

#include <EntityComponentSystem/World/World.h>

#include <GameEntities/CoinEntity.h>
#include <GameEntities/HealthEntity.h>

#include <Inventory/Items/ItemBase.h>

#include <Messages/Messages.h>

#include "GameData.h"

namespace ItemDropHandler
{
	
void Register(World& inWorld, MessageBroadcaster& inMsgBroadcaster, GameData* inGameData)
{
	inMsgBroadcaster.Register<MonsterDiedMsg>( [&, inGameData] (const MonsterDiedMsg& inMsg)
	{
		std::vector<ItemBase*> itemsNeedingAmmo;

		auto const& items = inGameData->mPlayerData.mInventory.GetAllItems();
		for (auto const& item : items)
		{
			if (item->RequiresAmmo())
			{
				itemsNeedingAmmo.push_back(item);
			}
		}

		size_t itemsSum = itemsNeedingAmmo.size() + 2; // + Health + Money

		size_t itemSelection = rand() % itemsSum;
		switch (itemSelection)
		{
			case 0: CoinEntity::Create(inWorld, inMsgBroadcaster, inMsg.mPosition); break;
			case 1: HealthEntity::Create(inWorld, inMsg.mPosition); break;
			default:
				itemSelection -= 2;
				itemsNeedingAmmo[itemSelection]->SpawnAmmo(inWorld, inMsg.mPosition);
				break;
		}
	} );
}

}
