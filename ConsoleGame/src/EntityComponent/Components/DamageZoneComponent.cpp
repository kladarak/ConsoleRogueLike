#include "DamageZoneComponent.h"

void DamageZoneComponent::SetDamageZoneAt(const IVec2& inPosition)
{
	mDamageZonePositions.push_back(inPosition);
}

void DamageZoneComponent::SetDamageZoneAt(const std::vector<IVec2>& inPositions)
{
	for (auto& pos : inPositions)
	{
		SetDamageZoneAt(pos);
	}
}
