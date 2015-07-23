#pragma once

class Entity;
class MessageBroadcaster;

class MonsterComponent
{
public:
	MonsterComponent();
	~MonsterComponent() { }
	
	void	UpdateDamageAnimations(Entity inThis, float inFrameTime, MessageBroadcaster& inMsgBroadcaster);
	void	StartDeath(const Entity& inThis);
	void	StartDamagedAnimation();

	bool	IsDying() const	{ return mIsDying; }
	
	void	OnEntityCollidedWith(Entity inThis, Entity inCollidingEntity) const;

private:
	float	mDamageAnimationTime;
	float	mTimeUntilDeath;
	bool	mIsDying;
};
