#pragma once

#include <EntityComponentSystem/World/World.h>

class HealthBarUI
{
public:
	HealthBarUI();

	void		Init(Entity inPlayer);
	std::string GetRenderBuffer() const;

private:
	Entity		mPlayer;
};
