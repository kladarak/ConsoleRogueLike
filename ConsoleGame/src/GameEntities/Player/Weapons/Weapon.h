#pragma once

#include <EntityComponentSystem/World/World.h>

#include <Animations/Animation.h>

#include "WeaponEnums.h"

class Weapon
{
public:
	Weapon(EWeapon inWeapon) : mName(inWeapon)	{ }
	virtual ~Weapon()							{ }

	EWeapon GetName() const						{ return mName; }

	virtual void					Attack(Entity inPlayer) = 0;
	virtual std::vector<Animation>	GetAnimations() const = 0;

private:
	EWeapon mName;

};
