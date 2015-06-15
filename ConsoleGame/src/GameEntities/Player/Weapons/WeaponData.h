#pragma once

#include <string>

#include <Containers/ContainerMacros.h>
#include <Renderer/AsciiMesh.h>

#include "WeaponEnums.h"

namespace WeaponData
{
	struct Data
	{
		EWeapon		mType;
		std::string mName;
		AsciiMesh	mHUDIcon;
	};
	
	static const char kSwordIcon[]	= {	'-','|','-','-','-' };
	static const char kBowIcon[]	= { 'D', ' ', 26 };

	static const Data kSwordData	= { EWeapon_Sword,	"Sword",	AsciiMesh(kSwordIcon,	gElemCount(kSwordIcon), 1) };
	static const Data kBowData		= { EWeapon_Bow,	"Bow",		AsciiMesh(kBowIcon,		gElemCount(kBowIcon),	1) };
}
