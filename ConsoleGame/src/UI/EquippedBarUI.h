#pragma once

#include <string>

class PlayerData;

class EquippedBarUI
{
public:
	EquippedBarUI();

	void		Init(PlayerData* inPlayerData);
	std::string GetRenderBuffer() const;

private:
	PlayerData*	mPlayerData;
};
