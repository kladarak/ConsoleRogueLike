#include "InventoryScreen.h"

#include <EntityComponent/Components/PlayerComponent.h>

#include <GameEntities/Player/Items/Inventory.h>
#include <GameEntities/Player/Items/ItemBase.h>

#include <Renderer/RenderTargetWriter.h>

void InventoryScreen::Init(Entity inPlayer)
{
	mPlayer = inPlayer;
}

void InventoryScreen::Update(float /*inFrameTime*/, const InputBuffer& /*inInput*/)
{
}

std::string InventoryScreen::GetRenderBuffer() const
{
	assert(mPlayer.IsValid());
	
	RenderTargetWriter renderTargetWriter(100, 50);

	auto& inventory = mPlayer.GetComponent<PlayerComponent>()->GetInventory();
	auto& items = inventory.GetAllItems();

	int row = 0;
	for (auto& item : items)
	{
		renderTargetWriter.Write(item->GetName(), 0, row++);
		
		const AsciiMesh& icon = item->GetHUDIcon();
		renderTargetWriter.Write(icon, 0, row);

		row += icon.GetLocalBounds().mHeight;
		row++;
	}

	return renderTargetWriter.GetRenderBuffer();
}
