#include "Inventory.h"

#include "Weapon.h"

Inventory::~Inventory()
{
	for (auto weapon : mWeapons)
	{
		delete weapon;
	}

	mWeapons.clear();
}

void Inventory::AddWeapon(Weapon* inWeapon)
{
	mWeapons.push_back(inWeapon);
}

Weapon* Inventory::GetWeapon(EWeapon inWeapon) const
{
	for (auto weapon : mWeapons)
	{
		if (weapon->GetName() == inWeapon)
		{
			return weapon;
		}
	}

	return nullptr;
}
