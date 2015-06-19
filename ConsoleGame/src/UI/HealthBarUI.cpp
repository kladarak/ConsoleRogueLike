#include "HealthBarUI.h"

#include <EntityComponent/Components/HealthComponent.h>

static const char kHeartIcon = 3;

HealthBarUI::HealthBarUI()
{
}

void HealthBarUI::Init(Entity inPlayer)
{
	 mPlayer = inPlayer;
}

std::string HealthBarUI::GetRenderBuffer() const
{
	auto health = mPlayer.GetComponent<HealthComponent>();
	int current = health->GetCurrentHealth();
	int max		= health->GetMaxHealth();

	std::string healthBar(max, '.');
	for (int i = 0; i < current; ++i)
	{
		healthBar[i] = kHeartIcon;
	}
	
	return healthBar;
}
