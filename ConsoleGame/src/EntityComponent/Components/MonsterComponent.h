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

	bool	IsDying() const { return mIsDying; }
	
	void	OnEntityCollidedWith(Entity inThis, Entity inCollidingEntity) const;

private:
	float	mTimeUntilDeath;
	bool	mIsDying;
};
