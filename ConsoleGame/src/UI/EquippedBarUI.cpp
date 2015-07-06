#include "EquippedBarUI.h"

#include <Inventory/Items/ItemBase.h>

#include <GameEntities/Player/PlayerData.h>

#include <Renderer/RenderTargetWriter.h>

EquippedBarUI::EquippedBarUI()
	: mPlayerData(nullptr)
{
}

void EquippedBarUI::Init(PlayerData* inPlayerData)
{
	mPlayerData = inPlayerData;
}

std::string EquippedBarUI::GetRenderBuffer() const
{
	RenderTargetWriter renderHelper(20, 2);

	auto item0 = mPlayerData->GetItemInSlot( Player::EItemSlot_Slot0 );
	auto item1 = mPlayerData->GetItemInSlot( Player::EItemSlot_Slot1 );

	renderHelper.Write("Spc:",	0, 0);
	renderHelper.Write("E:",	0, 1);

	if (nullptr != item0)
	{
		renderHelper.Write(item0->GetHUDIcon(), 5, 0);
	}

	if (nullptr != item1)
	{
		renderHelper.Write(item1->GetHUDIcon(), 5, 1);
	}

	return renderHelper.GetRenderBuffer();
}
