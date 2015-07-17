#include "InventoryScreenState.h"

#include <EntityComponent/Components/PlayerComponent.h>

#include <Inventory/Inventory.h>
#include <Inventory/Items/ItemBase.h>

#include <Input/InputBuffer.h>
#include <Renderer/RenderTargetWriter.h>
#include "GameData.h"


InventoryScreenState::InventoryScreenState(MessageBroadcaster* inStateMachineMsgBroadcaster, GameData* inGameData)
	: StateBase(inStateMachineMsgBroadcaster, inGameData)
	, mHighlightedItem(0)
{
}

void InventoryScreenState::Update(float /*inFrameTime*/, const InputBuffer& inInput)
{
	auto&	items		= mGameData->mPlayerData.mInventory.GetAllItems();
	size_t	itemCount	= items.size();

	if (inInput.IsPressed('w'))
	{
		mHighlightedItem += (itemCount - 1);
		mHighlightedItem %= itemCount;
	}
	else if (inInput.IsPressed('s'))
	{
		mHighlightedItem++;
		mHighlightedItem %= itemCount;
	}
	
	if (inInput.IsPressed(' '))
	{
		mGameData->mPlayerData.SetItemInSlot( items[mHighlightedItem], Player::EItemSlot_Slot0 );
	}
	
	if (inInput.IsPressed('e'))
	{
		mGameData->mPlayerData.SetItemInSlot( items[mHighlightedItem], Player::EItemSlot_Slot1 );
	}

	if (inInput.IsPressed('\t'))
	{
		RequestPopState();
	}
}

RenderTarget InventoryScreenState::GetRenderTarget() const
{
	assert(mPlayer.IsValid());
	
	RenderTargetWriter renderTargetWriter(50, 20);

	auto	armedItem0	= mGameData->mPlayerData.GetItemInSlot( Player::EItemSlot_Slot0 );
	auto	armedItem1	= mGameData->mPlayerData.GetItemInSlot( Player::EItemSlot_Slot1 );
	auto&	items		= mGameData->mPlayerData.mInventory.GetAllItems();
	
	int row = 0;
	int col = 4;
	for (size_t i = 0; i < items.size(); ++i)
	{
		if (i == mHighlightedItem)
		{
			renderTargetWriter.Write( (char) 26, 2, row );
		}

		auto item = items[i];

		std::string itemName = item->GetName();
		itemName += (item == armedItem0) ? " (Spc)" : "";
		itemName += (item == armedItem1) ? " (E)"	: "";
		renderTargetWriter.Write(itemName, ETextWhite, col, row++);
		
		const AsciiMesh& icon = item->GetHUDIcon();
		renderTargetWriter.Write(icon, col, row);
	
		row += icon.GetLocalBounds().mHeight;
		row++;
	}

	return renderTargetWriter.GetRenderTarget();
}
