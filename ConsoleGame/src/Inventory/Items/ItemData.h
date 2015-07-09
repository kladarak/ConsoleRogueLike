#pragma once

#include <string>

#include <Renderer/AsciiMesh.h>

enum EAmmoRequirement
{
	ERequiresNoAmmo,
	ERequiresAmmo,
};

struct ItemData
{
	ItemData(const std::string& inName, const AsciiMesh& inMesh, EAmmoRequirement inAmmoReq)
		: mName				(inName)
		, mHUDIcon			(inMesh)
		, mAmmoRequirement	(inAmmoReq)
	{
	}

	std::string			mName;
	AsciiMesh			mHUDIcon;
	EAmmoRequirement	mAmmoRequirement;
};

