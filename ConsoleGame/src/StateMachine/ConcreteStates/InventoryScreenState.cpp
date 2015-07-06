#include "InventoryScreenState.h"

#include <EntityComponent/Components/PlayerComponent.h>

#include <GameEntities/Player/Items/Inventory.h>
#include <GameEntities/Player/Items/ItemBase.h>

#include <Input/InputBuffer.h>
#include <Renderer/RenderTargetWriter.h>


InventoryScreenState::InventoryScreenState()
	: mHighlightedItem(0)
{
}

void InventoryScreenState::Init(Entity inPlayer)
{
	mPlayer = inPlayer;
}

void InventoryScreenState::Update(float /*inFrameTime*/, const InputBuffer& inInput)
{
	// TODO: Handle user input for state changes, e.g., tab.

	auto	playerComp	= mPlayer.GetComponent<PlayerComponent>();
	auto&	items		= playerComp->GetInventory().GetAllItems();
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
		playerComp->SetItemInSlot( items[mHighlightedItem], Player::EItemSlot_Slot0 );
	}
	
	if (inInput.IsPressed('e'))
	{
		playerComp->SetItemInSlot( items[mHighlightedItem], Player::EItemSlot_Slot1 );
	}
}

std::string InventoryScreenState::GetRenderBuffer() const
{
	assert(mPlayer.IsValid());
	
	RenderTargetWriter renderTargetWriter(50, 20);

	auto	playerComp	= mPlayer.GetComponent<PlayerComponent>();
	auto&	inventory	= playerComp->GetInventory();
	auto	armedItem0	= playerComp->GetItemInSlot( Player::EItemSlot_Slot0 );
	auto	armedItem1	= playerComp->GetItemInSlot( Player::EItemSlot_Slot1 );
	auto&	items		= inventory.GetAllItems();
	
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
		renderTargetWriter.Write(itemName, col, row++);
		
		const AsciiMesh& icon = item->GetHUDIcon();
		renderTargetWriter.Write(icon, col, row);
	
		row += icon.GetLocalBounds().mHeight;
		row++;
	}

	return renderTargetWriter.GetRenderBuffer();
}
