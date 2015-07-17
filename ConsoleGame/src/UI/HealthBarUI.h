#pragma once

#include <EntityComponentSystem/World/World.h>
#include <Renderer/RenderTarget.h>

class HealthBarUI
{
public:
	HealthBarUI();

	void			Init(Entity inPlayer);
	RenderTarget	GetRenderTarget() const;

private:
	Entity			mPlayer;
};
