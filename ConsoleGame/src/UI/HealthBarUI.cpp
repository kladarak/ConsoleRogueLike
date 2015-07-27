#include "HealthBarUI.h"

#include <EntityComponent/Components/HealthComponent.h>


static const Fragment kFullHeartIcon( 3, ETextRed );
static const Fragment kEmptyHeartIcon( gCastUCharToChar(250), ETextRed );

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
	
	RenderTarget renderTarget(10, 2);

	for (int i = 0; i < max; ++i)
	{
		const Fragment& heart = (i < current) ? kFullHeartIcon : kEmptyHeartIcon;
		renderTarget.Write(heart, i % 10, (i / 10));
	}
	
	return renderTarget;
}
