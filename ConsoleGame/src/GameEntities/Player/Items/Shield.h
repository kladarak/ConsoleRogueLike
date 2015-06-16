#pragma once

#include "ItemBase.h"

class Shield : public ItemBase
{
public:
	Shield();
	
	virtual void					Use(Entity inPlayer, bool inStartedUsingThisFrame);
	virtual std::vector<Animation>	GetAnimations() const;

private:

};
