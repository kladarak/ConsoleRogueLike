#include "Weapon.h"

class Sword : public Weapon
{
public:
	Sword();

	virtual void					Attack(Entity inPlayer, bool inStartedAttackThisFrame);
	virtual std::vector<Animation>	GetAnimations() const;

private:

};
