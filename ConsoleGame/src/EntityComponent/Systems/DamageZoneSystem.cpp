#include "DamageZoneSystem.h"

#include <EntityComponentSystem/World/World.h>
#include <EntityComponentSystem/World/EntityFilter.h>

#include <EntityComponent/Components/DamageZoneComponent.h>
#include <EntityComponent/Components/PositionComponent.h>

namespace DamageZoneSystem
{

bool IsDamageZone(World& inWorld, const IVec2& inPosition)
{
	auto damageZones = inWorld.GetEntities( EntityFilter().MustHave<DamageZoneComponent>() );

	for (auto& damageZone : damageZones)
	{
		if (IsDamageZone(damageZone, inPosition))
		{
			return true;
		}
	}

	return false;
}

bool IsDamageZone(World& inWorld, Entity inExceptThis, const IVec2& inPosition)
{
	auto damageZones = inWorld.GetEntities( EntityFilter().MustHave<DamageZoneComponent>() );

	for (auto& damageZone : damageZones)
	{
		if (damageZone != inExceptThis && IsDamageZone(damageZone, inPosition))
		{
			return true;
		}
	}

	return false;
}

bool IsDamageZone(Entity inDamageZoneEntity, const IVec2& inPosition)
{
	auto damageZoneComp	= inDamageZoneEntity.GetComponent<DamageZoneComponent>();
	if (nullptr == damageZoneComp)
	{
		false;
	}

	auto	positionComp		= inDamageZoneEntity.GetComponent<PositionComponent>();
	IVec2	damageZoneOffset	= (nullptr != positionComp) ? positionComp->GetPosition() : IVec2(0, 0);
	auto&	damageZonePositions	= damageZoneComp->GetDamageZonePositions();

	for (auto& position : damageZonePositions)
	{
		if ((position + damageZoneOffset) == inPosition)
		{
			return true;
		}
	}

	return false;
}

}
