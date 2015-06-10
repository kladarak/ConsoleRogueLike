#include "TriggerBoxComponent.h"

#include <EntityComponentSystem/World/World.h>

#include "PositionComponent.h"

TriggerBoxComponent::TriggerBoxComponent(TriggerBoxComponent&& inRHS)
	: mBounds						( inRHS.mBounds )
	, mOnEnterCallbacks				( std::move(inRHS.mOnEnterCallbacks) )
	, mOnExitCallbacks				( std::move(inRHS.mOnExitCallbacks) )
{
}

bool TriggerBoxComponent::IsInBounds(const Entity& inThis, const IVec2& inPosition) const
{
	IVec2 position = inPosition;

	auto thisPosComp = inThis.GetComponent<PositionComponent>();
	if (nullptr != thisPosComp)
	{
		position = position - thisPosComp->GetPosition();
	}

	return mBounds.Contains(position);
}

void TriggerBoxComponent::OnEntered(const Entity& inThis, const Entity& inTriggerer)
{
	for (auto& callback : mOnEnterCallbacks)
	{
		callback(inThis, inTriggerer);
	}
}

void TriggerBoxComponent::OnExited(const Entity& inThis, const Entity& inTriggerer)
{
	for (auto& callback : mOnExitCallbacks)
	{
		callback(inThis, inTriggerer);
	}
}
