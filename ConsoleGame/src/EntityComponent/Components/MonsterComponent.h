#pragma once

class Entity;
class MessageBroadcaster;

class MonsterComponent
{
public:
	MonsterComponent();
	~MonsterComponent() { }
	
	void	Update(Entity inThis, float inFrameTime, MessageBroadcaster& inMsgBroadcaster);
	void	StartDeath(const Entity& inThis);

	void	SetPassiveWhenTouched(bool inPassive)	{ mPassiveWhenTouched = inPassive; }

	bool	IsDying() const							{ return mIsDying; }
	bool	IsPassiveWhenTouched() const			{ return mPassiveWhenTouched; }
	
	void	OnEntityCollidedWith(Entity inThis, Entity inCollidingEntity) const;

private:
	float	mTimeUntilDeath;
	bool	mIsDying;
	bool	mPassiveWhenTouched;
};
