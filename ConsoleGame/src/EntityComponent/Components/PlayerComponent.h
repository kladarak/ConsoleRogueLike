#pragma once

#include <GameEntities/Player/PlayerEnums.h>

class PlayerComponent
{
public:
	PlayerComponent(Player::EFacingDirection inDirection) : mFacingDirection(inDirection), mState(Player::EState_Idle) { }
	~PlayerComponent() { }
	
	Player::EFacingDirection	GetFacingDirection() const										{ return mFacingDirection; }
	void						SetFacingDirection(Player::EFacingDirection inFacingDirection)	{ mFacingDirection = inFacingDirection; }

	Player::EState				GetState() const												{ return mState; }
	void						SetState(Player::EState inState) 								{ mState = inState; }

private:
	Player::EFacingDirection	mFacingDirection;
	Player::EState				mState;

};
