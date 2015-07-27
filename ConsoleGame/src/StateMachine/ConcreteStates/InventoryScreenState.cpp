#include "InventoryScreenState.h"

#include <EntityComponent/Components/PlayerComponent.h>

#include <Inventory/Inventory.h>
#include <Inventory/Items/ItemBase.h>

#include <Input/InputBuffer.h>

#include <UI/Elements/BorderedFrame.h>

#include <ScreenConstants.h>
#include "GameData.h"


InventoryScreenState::InventoryScreenState(MessageBroadcaster* inStateMachineMsgBroadcaster, GameData* inGameData)
	: StateBase(inStateMachineMsgBroadcaster, inGameData)
	, mHighlightedItem(0)
{
}

void InventoryScreenState::Update(float /*inFrameTime*/, const InputBuffer& inInput)
{
	auto&	items		= mGameData->mPlayerData.mInventory.GetAllItems();
	int		itemCount	= (int) items.size();

	if (inInput.IsPressed('w'))
	{
		mHighlightedItem -= 2;
		
		if (mHighlightedItem < 0)
		{
			if (mHighlightedItem % 2 == 0)
			{
				mHighlightedItem += (itemCount % 2); 
			}
			else
			{
				mHighlightedItem -= (itemCount % 2); 
			}

			mHighlightedItem += itemCount;
		}
	}
	else if (inInput.IsPressed('s'))
	{
		mHighlightedItem += 2;

		if (mHighlightedItem >= itemCount)
		{
			if (mHighlightedItem % 2 == 0)
			{
				mHighlightedItem -= (itemCount % 2); 
			}
			else
			{
				mHighlightedItem += (itemCount % 2); 
			}

			mHighlightedItem -= itemCount;
		}
	}

	if (inInput.IsPressed('a') || inInput.IsPressed('d'))
	{
		// if odd number of items, and selecting the last item, don't do this.
		if (itemCount % 2 == 0 || mHighlightedItem != itemCount - 1)
		{
			mHighlightedItem += (mHighlightedItem % 2) == 0 ? 1 : -1;
		}
	}
	
	if (inInput.IsPressed(' '))
	{
		if (mHighlightedItem < itemCount) // May be 1 more than max due to layout.
		{
			mGameData->mPlayerData.SetItemInSlot( items[mHighlightedItem], Player::EItemSlot_Slot0 );
		}
	}
	
	if (inInput.IsPressed('e'))
	{
		if (mHighlightedItem < itemCount) // May be 1 more than max due to layout.
		{
			mGameData->mPlayerData.SetItemInSlot( items[mHighlightedItem], Player::EItemSlot_Slot1 );
		}
	}

	if (inInput.IsPressed('\t'))
	{
		RequestPopState();
	}
}

RenderTarget InventoryScreenState::GetRenderTarget() const
{
	assert(mPlayer.IsValid());
	
	static const int		kWindowWidth					= ScreenConstants::EViewPortWidth;
	static const int		kWindowHeight					= ScreenConstants::EViewPortHeight;
	
	static const int		kWindowMargin					= 2;
	
	static const int		kCellLeftOffset					= 2;
	static const int		kCellWidth						= (ScreenConstants::EViewPortWidth - (2 * (kWindowMargin + kCellLeftOffset)) - 2) / 2;
	static const int		kCellHeight						= 2;
	static const int		kVerticalSpacingBetweenCells	= 1;

	static const Fragment	kCursor((char) 26, ETextWhite);
	
	auto	armedItem0	= mGameData->mPlayerData.GetItemInSlot( Player::EItemSlot_Slot0 );
	auto	armedItem1	= mGameData->mPlayerData.GetItemInSlot( Player::EItemSlot_Slot1 );
	auto&	items		= mGameData->mPlayerData.mInventory.GetAllItems();

	auto renderItemCell = [&] (ItemBase* inItem)
	{
		RenderTarget target(kCellWidth, kCellHeight);

		std::string itemName = inItem->GetName();
		itemName += (inItem == armedItem0) ? " (Spc)" : "";
		itemName += (inItem == armedItem1) ? " (E)"	: "";

		target.Write(itemName, ETextWhite, 0, 0);
		target.Write(inItem->GetHUDIcon(), 0, 1);

		return target;
	};

	RenderTarget window = BorderedFrame(kWindowWidth, kWindowHeight).Render();
	window.Write( " Inventory ", 3, 0 );
	
	for (size_t i = 0; i < items.size(); ++i)
	{
		RenderTarget renderedCell = renderItemCell( items[i] );

		int col = ((i % 2) == 0) ? (kWindowMargin + kCellLeftOffset) : (kWindowWidth - kWindowMargin - kCellWidth);
		int row = kWindowMargin + ((i / 2) * (kCellHeight + kVerticalSpacingBetweenCells));

		window.Write(renderedCell, col, row);

		if ((int) i == mHighlightedItem)
		{
			window.Write(kCursor, col - kCellLeftOffset, row);
		}
	}

	return window;
}
