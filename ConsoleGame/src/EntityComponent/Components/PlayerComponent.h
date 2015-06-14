#pragma once

#include <Maths/IVec2.h>
#include <GameEntities/Player/PlayerEnums.h>

class PlayerComponent
{
public:
	PlayerComponent(Player::EFacingDirection inDirection) 
		: mFacingDirection(inDirection)
		, mState(Player::EState_Idle)
		, mIntendedMovement(0, 0)
		, mDamaged(false)
	{
	}

	~PlayerComponent() { }
	
	Player::EFacingDirection	GetFacingDirection() const										{ return mFacingDirection; }
	void						SetFacingDirection(Player::EFacingDirection inFacingDirection)	{ mFacingDirection = inFacingDirection; }

	Player::EState				GetState() const												{ return mState; }
	void						SetState(Player::EState inState) 								{ mState = inState; }

	void						SetIntendedMovement(const IVec2& inMovement)					{ mIntendedMovement = inMovement; }
	const IVec2&				GetIntendedMovement() const										{ return mIntendedMovement; }

	void						SetDamaged(bool inValue)										{ mDamaged = inValue; }
	bool						IsDamaged() const												{ return mDamaged; }

private:
	Player::EFacingDirection	mFacingDirection;
	Player::EState				mState;
	IVec2						mIntendedMovement;
	bool						mDamaged;
};
