#include "HealthBarUI.h"

#include <EntityComponent/Components/HealthComponent.h>
#include <Renderer/RenderTargetWriter.h>

static const char kHeartIcon = 3;

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

	std::string healthBar(max, '.');
	for (int i = 0; i < current; ++i)
	{
		healthBar[i] = kHeartIcon;
	}
	
	writer.Write(healthBar, 0, 0);

	return writer.GetRenderTarget();
}
