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

	virtual void					OnStartUsing(Entity inPlayer)					= 0;
	virtual bool					UpdateUsing(Entity inPlayer, float inFrameTime)	= 0;
	virtual void					OnStoppedUsing(Entity inPlayer)					= 0;

private:
	ItemData mData;

};
