#pragma once

#include <vector>
#include <Maths/IVec2.h>

class Entity;

class DamageZoneComponent
{
public:
	DamageZoneComponent()	{ }
	~DamageZoneComponent()	{ }
	DamageZoneComponent(DamageZoneComponent&& inRHS)		: mDamageZonePositions( std::move(inRHS.mDamageZonePositions) )	{ }
	DamageZoneComponent(const DamageZoneComponent& inRHS)	: mDamageZonePositions( inRHS.mDamageZonePositions )			{ }
	
	void						SetDamageZoneAt(int inX, int inY)			{ SetDamageZoneAt(IVec2(inX, inY)); }
	void						SetDamageZoneAt(const IVec2& inPosition);
	void						SetDamageZoneAt(const std::vector<IVec2>& inPositions);

	const std::vector<IVec2>&	GetDamageZonePositions() const				{ return mDamageZonePositions; }

private:
	std::vector<IVec2>			mDamageZonePositions;
};
