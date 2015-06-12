#pragma once

class HealthComponent
{
public:
	HealthComponent(int inMaxHealth) : mCurrentHealth(inMaxHealth), mMaxHealth(inMaxHealth) { }
	~HealthComponent() { }

	int		GetMaxHealth() const			{ return mMaxHealth;		}
	void	SetMaxHealth(int inValue)		{ mMaxHealth = inValue;		}
	
	int		GetCurrentHealth() const		{ return mCurrentHealth;	}
	void	SetCurrentHealth(int inValue)	{ mCurrentHealth = inValue; }

	void	IncHealth()						{ if (!IsFullHealth())	{ ++mCurrentHealth; } }
	void	DecHealth()						{ if (!IsDead())		{ --mCurrentHealth; } }

	bool	IsDead() const					{ return GetCurrentHealth() == 0;				}
	bool	IsFullHealth() const			{ return GetCurrentHealth() == GetMaxHealth();	}

private:
	int		mCurrentHealth;
	int		mMaxHealth;

};