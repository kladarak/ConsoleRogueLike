#include "EquippedBarUI.h"

#include <EntityComponent/Components/PlayerComponent.h>

#include <GameEntities/Player/Items/ItemBase.h>

#include <Renderer/RenderTargetWriter.h>

EquippedBarUI::EquippedBarUI()
{
}

void EquippedBarUI::Init(Entity inPlayer)
{
	 mPlayer = inPlayer;
}

std::string EquippedBarUI::GetRenderBuffer() const
{
	RenderTargetWriter renderHelper(20, 2);

	auto playerComp = mPlayer.GetComponent<PlayerComponent>();
	auto item0 = playerComp->GetItemInSlot( Player::EItemSlot_Slot0 );
	auto item1 = playerComp->GetItemInSlot( Player::EItemSlot_Slot1 );

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
