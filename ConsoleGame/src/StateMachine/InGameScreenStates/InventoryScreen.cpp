#include "InventoryScreen.h"

#include <EntityComponent/Components/PlayerComponent.h>

#include <GameEntities/Player/Items/Inventory.h>
#include <GameEntities/Player/Items/ItemBase.h>

#include <Input/InputBuffer.h>
#include <Renderer/RenderTargetWriter.h>


InventoryScreen::InventoryScreen()
	: mHighlightedItem(0)
{
}

void InventoryScreen::Init(Entity inPlayer)
{
	mPlayer = inPlayer;
}

void InventoryScreen::Update(float /*inFrameTime*/, const InputBuffer& inInput)
{
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
		playerComp->SetItemInSlot1( items[mHighlightedItem] );
	}
	
	if (inInput.IsPressed('e'))
	{
		playerComp->SetItemInSlot2( items[mHighlightedItem] );
	}
}

std::string InventoryScreen::GetRenderBuffer() const
{
	assert(mPlayer.IsValid());
	
	RenderTargetWriter renderTargetWriter(50, 20);

	auto	playerComp	= mPlayer.GetComponent<PlayerComponent>();
	auto&	inventory	= playerComp->GetInventory();
	auto	armedItem1	= playerComp->GetItemInSlot1();
	auto	armedItem2	= playerComp->GetItemInSlot2();
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
		itemName += (item == armedItem1) ? " (Spc)" : "";
		itemName += (item == armedItem2) ? " (E)"	: "";
		renderTargetWriter.Write(itemName, col, row++);
		
		const AsciiMesh& icon = item->GetHUDIcon();
		renderTargetWriter.Write(icon, col, row);
	
		row += icon.GetLocalBounds().mHeight;
		row++;
	}

	return renderTargetWriter.GetRenderBuffer();
}
