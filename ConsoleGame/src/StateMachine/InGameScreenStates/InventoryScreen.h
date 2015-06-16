#pragma once

#include <string>
#include <EntityComponentSystem/World/World.h>

class InputBuffer;

class InventoryScreen
{
public:
	InventoryScreen();

	void		Init(Entity inPlayer);
	void		Update(float inFrameTime, const InputBuffer& inInput);
	std::string GetRenderBuffer() const;

private:
	Entity		mPlayer;
	uint32_t	mHighlightedItem;
};
