#pragma once

#include <EntityComponentSystem/World/World.h>

#include <Animations/Animation.h>

#include "ItemData.h"

class ItemBase
{
public:
	ItemBase(const ItemData& inData) : mData(inData)	{ }
	virtual ~ItemBase()									{ }

	const std::string&				GetName() const			{ return mData.mName;		}
	const AsciiMesh&				GetHUDIcon() const		{ return mData.mHUDIcon;	}

	virtual void					Use(Entity inPlayer, bool inStartedUsingThisFrame)	= 0;
	virtual std::vector<Animation>	GetAnimations() const								= 0;

private:
	ItemData mData;

};
