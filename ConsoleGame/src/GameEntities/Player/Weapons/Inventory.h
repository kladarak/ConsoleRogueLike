#pragma once

#include <vector>
#include "WeaponEnums.h"

class Weapon;

class Inventory
{
public:
	Inventory()																	{ }
	Inventory(Inventory&& inRHS)		: mWeapons( std::move(inRHS.mWeapons) )	{ }
	Inventory(const Inventory& inRHS)	: mWeapons( inRHS.mWeapons )			{ }
	~Inventory();

	void						AddWeapon(Weapon* inWeapon);

	Weapon*						GetWeapon(EWeapon inWeapon) const;
	const std::vector<Weapon*>& GetAllWeapons() const { return mWeapons; }

private:
	std::vector<Weapon*>		mWeapons;

};
