#pragma once

#include <EntityComponentSystem/World/World.h>

#include <Animations/Animation.h>

#include "WeaponEnums.h"
#include "WeaponData.h"

class Weapon
{
public:
	Weapon(const WeaponData::Data& inData) : mData(inData)	{ }
	virtual ~Weapon()										{ }

	EWeapon							GetWeaponType() const	{ return mData.mType;		}
	const std::string&				GetName() const			{ return mData.mName;		}
	const AsciiMesh&				GetHUDIcon() const		{ return mData.mHUDIcon;	}

	virtual void					Attack(Entity inPlayer, bool inStartedAttackThisFrame) = 0;
	virtual std::vector<Animation>	GetAnimations() const = 0;

private:
	WeaponData::Data mData;

};
