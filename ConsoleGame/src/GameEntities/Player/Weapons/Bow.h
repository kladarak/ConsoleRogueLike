#pragma once

#include "Weapon.h"

class Bow : public Weapon
{
public:
	Bow();
	
	virtual void					Attack(Entity inPlayer, bool inStartedAttackThisFrame);
	virtual std::vector<Animation>	GetAnimations() const;

private:

};
