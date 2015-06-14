#include "Weapon.h"

class Sword : public Weapon
{
public:
	Sword() : Weapon(EWeapon_Sword) { }

	virtual void					Attack(Entity inPlayer);
	virtual std::vector<Animation>	GetAnimations() const;

private:
};
