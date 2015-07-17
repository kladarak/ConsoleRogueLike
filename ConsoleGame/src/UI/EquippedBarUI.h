#pragma once

#include <string>
#include <Renderer/RenderTarget.h>

class PlayerData;

class EquippedBarUI
{
public:
	EquippedBarUI();

	void			Init(PlayerData* inPlayerData);
	RenderTarget	GetRenderTarget() const;

private:
	PlayerData*		mPlayerData;
};
