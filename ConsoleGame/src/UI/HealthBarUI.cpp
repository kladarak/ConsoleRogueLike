#include "HealthBarUI.h"

#include <EntityComponent/Components/HealthComponent.h>
#include <Renderer/RenderTargetWriter.h>

static const Fragment kHeartIcon( 3, ETextRed );

HealthBarUI::HealthBarUI()
{
}

void HealthBarUI::Init(Entity inPlayer)
{
	 mPlayer = inPlayer;
}

RenderTarget HealthBarUI::GetRenderTarget() const
{
	auto health = mPlayer.GetComponent<HealthComponent>();
	int current = health->GetCurrentHealth();
	int max		= health->GetMaxHealth();
	
	RenderTargetWriter writer(max, 1);

	for (int i = 0; i < current; ++i)
	{
		writer.Write(kHeartIcon, i, 0);
	}
	
	return writer.GetRenderTarget();
}
