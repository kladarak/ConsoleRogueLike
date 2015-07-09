#pragma once

#include <Maths/IRect.h>
#include <EntityComponentSystem/World/World.h>

#include <functional>
#include <vector>

class Entity;

class TriggerBoxComponent
{
public:
	TriggerBoxComponent()						: mBounds(0, 0, 1, 1)	{ }
	TriggerBoxComponent(const IRect& inRect)	: mBounds(inRect)		{ }
	TriggerBoxComponent(TriggerBoxComponent&& inRHS);
	~TriggerBoxComponent()												{ }

	typedef std::function<void (const Entity& inTriggerBox, const Entity& inTriggerer)> TriggerCallback;

	// TODO: Handle unregistering.
	void							RegisterOnEnterCallback(const TriggerCallback& inOnEnter)						{ mOnEnterCallbacks.push_back(inOnEnter);	}
	void							RegisterOnExitCallback(const TriggerCallback& inOnExit)							{ mOnExitCallbacks.push_back(inOnExit);		}
	
	bool							IsInBounds(const Entity& inThis, const IVec2& inPosition) const;
	const IRect&					GetBounds() const																{ return mBounds; }

	void							OnEntered(const Entity& inThis, const Entity& inTriggerer);
	void							OnExited(const Entity& inThis, const Entity& inTriggerer);
	
	const std::vector<Entity>&		GetEntitiesInBoundsLastFrame() const											{ return mEntitiesInBoundsLastFrame; }
	void							SetEntitiesInBoundsLastFrame(const std::vector<Entity>& inEntitiesInBounds)		{ mEntitiesInBoundsLastFrame = inEntitiesInBounds; }

private:
	IRect							mBounds;

	std::vector<TriggerCallback>	mOnEnterCallbacks;
	std::vector<TriggerCallback>	mOnExitCallbacks;

	std::vector<Entity>				mEntitiesInBoundsLastFrame;
};

class TriggererComponent
{
public:
	~TriggererComponent() { }
	// No functionality: it's just to indicate what entities can trigger triggerboxes.
};
