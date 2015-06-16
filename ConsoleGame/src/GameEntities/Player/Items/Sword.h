#include "ItemBase.h"

class Sword : public ItemBase
{
public:
	Sword();

	virtual void					Use(Entity inPlayer, bool inStartedAttackThisFrame);
	virtual std::vector<Animation>	GetAnimations() const;

private:

};
