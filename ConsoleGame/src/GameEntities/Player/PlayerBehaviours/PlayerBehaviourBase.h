#pragma once

#include <EntityComponentSystem/World/World.h>

struct IVec2;

class PlayerBehaviourBase
{
public:
	virtual void ApplyComponentModifiers(Entity inPlayer);

	virtual void Update(Entity inPlayer, float inFrameTime);
	
	virtual bool CanMoveToPosition(Entity inPlayer, const IVec2& inPosition);

};

typedef PlayerBehaviourBase PlayerIdleBehaviour;