#pragma once

class Entity;
class AttackMsg;

namespace Player
{
	void UpdatePlayer(const Entity& inPlayer, float inFrameTime);
	void OnAttacked(const Entity& inPlayer, const AttackMsg& inMsg);
}
