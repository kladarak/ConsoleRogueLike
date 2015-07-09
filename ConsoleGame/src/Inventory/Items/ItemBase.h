#pragma once

#include <EntityComponentSystem/World/World.h>

#include <Animations/Animation.h>

#include "ItemData.h"

#include <GameEntities/Player/PlayerBehaviours/PlayerBehaviourBase.h>

class ItemBase
{
public:
	ItemBase(const ItemData& inData) : mData(inData)		{ }
	virtual ~ItemBase()										{ }

	const std::string&				GetName() const			{ return mData.mName;		}
	const AsciiMesh&				GetHUDIcon() const		{ return mData.mHUDIcon;	}
	bool							RequiresAmmo() const	{ return mData.mAmmoRequirement == ERequiresAmmo; }

	virtual int						GetAmmoCount() const							{ return -1; }
	virtual void					SpawnAmmo(World&, const IVec2& /*inPosition*/)	{ }

	virtual PlayerBehaviourBase*	GetPlayerBehaviour() = 0;

private:
	ItemData						mData;

};
