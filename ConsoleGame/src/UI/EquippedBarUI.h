#pragma once

#include <EntityComponentSystem/World/World.h>

class EquippedBarUI
{
public:
	EquippedBarUI();

	void		Init(Entity inPlayer);
	std::string GetRenderBuffer() const;

private:
	Entity		mPlayer;
};
