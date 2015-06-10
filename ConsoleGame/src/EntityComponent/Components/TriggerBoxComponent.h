#pragma once

#include <Maths/IRect.h>
#include <EntityComponentSystem/World/World.h>

#include <functional>
#include <vector>

class Entity;

class TriggerBoxComponent
{
public:
	TriggerBoxComponent(const IRect& inRect) : mBounds(inRect)	{ }
	TriggerBoxComponent(TriggerBoxComponent&& inRHS);
	~TriggerBoxComponent()										{ }

	typedef std::function<void (const Entity& inTriggerBox, const Entity& inTriggerer)> TriggerCallback;

	// TODO: Handle unregistering.
	void							RegisterOnEnterCallback(const TriggerCallback& inOnEnter)				{ mOnEnterCallbacks.push_back(inOnEnter);	}
	void							RegisterOnExitCallback(const TriggerCallback& inOnExit)					{ mOnExitCallbacks.push_back(inOnExit);		}
	
	bool							IsInBounds(const Entity& inThis, const IVec2& inPosition) const;
	
	void							OnEntered(const Entity& inThis, const Entity& inTriggerer);
	void							OnExited(const Entity& inThis, const Entity& inTriggerer);
	
private:
	IRect							mBounds;

	std::vector<TriggerCallback>	mOnEnterCallbacks;
	std::vector<TriggerCallback>	mOnExitCallbacks;
};

class TriggererComponent
{
public:
	~TriggererComponent() { }
	// No functionality: it's just to indicate what entities can trigger triggerboxes.
};
