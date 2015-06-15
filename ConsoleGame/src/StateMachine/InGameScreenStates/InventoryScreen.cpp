#include "InventoryScreen.h"

#include <EntityComponent/Components/PlayerComponent.h>

#include <GameEntities/Player/Weapons/Inventory.h>
#include <GameEntities/Player/Weapons/Weapon.h>

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
	auto& weapons = inventory.GetAllWeapons();

	int row = 0;
	for (auto& weapon : weapons)
	{
		renderTargetWriter.Write(weapon->GetName(), 0, row++);
		
		const AsciiMesh& icon = weapon->GetHUDIcon();
		renderTargetWriter.Write(icon, 0, row);

		row += icon.GetLocalBounds().mHeight;
		row++;
	}

	return renderTargetWriter.GetRenderBuffer();
}
