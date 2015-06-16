#pragma once

#include "ItemBase.h"

class Bow : public ItemBase
{
public:
	Bow();
	
	virtual void					Use(Entity inPlayer, bool inStartedAttackThisFrame);
	virtual std::vector<Animation>	GetAnimations() const;

private:

};
