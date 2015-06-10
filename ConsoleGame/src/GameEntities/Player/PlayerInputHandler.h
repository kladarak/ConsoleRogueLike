#pragma once

class Entity;
class InputBuffer;

namespace Player
{
	void HandleInput(const Entity& inPlayer, const InputBuffer& inBuffer);
}
