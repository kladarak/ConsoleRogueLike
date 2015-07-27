#include "EquippedBarUI.h"

#include <Inventory/Items/ItemBase.h>

#include <GameEntities/Player/PlayerData.h>



EquippedBarUI::EquippedBarUI()
	: mPlayerData(nullptr)
{
}

void EquippedBarUI::Init(PlayerData* inPlayerData)
{
	mPlayerData = inPlayerData;
}

RenderTarget EquippedBarUI::GetRenderTarget() const
{
	using namespace Player;

	RenderTarget renderTarget(20, 2);

	renderTarget.Write("Spc:",	ETextWhite, 0, 0);
	renderTarget.Write("E:",	ETextWhite, 0, 1);

	static const int kXOffset = 5;

	auto renderItemIcon = [&] (EItemSlot inSlot)
	{
		auto item = mPlayerData->GetItemInSlot( inSlot );
		if (nullptr != item)
		{
			const int y = inSlot;

			auto const& icon = item->GetHUDIcon();
			renderTarget.Write(icon, kXOffset, y);

			int ammoCount = item->GetAmmoCount();
			if (ammoCount >= 0)
			{
				int width = icon.GetLocalBounds().mWidth;
				int ammoXOffset = kXOffset + 1 + width;
				std::string ammoAsStr = "x" + std::to_string(ammoCount);
				renderTarget.Write(ammoAsStr, ETextWhite, ammoXOffset, y);
			}
		}
	};

	renderItemIcon( EItemSlot_Slot0 );
	renderItemIcon( EItemSlot_Slot1 );

	return renderTarget;
}
