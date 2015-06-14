#pragma once

#include "PlayerEnums.h"

class PlayerUpdateState
{
public:
	PlayerUpdateState()
		: mLastState(Player::EState_Idle)
		, mLastFacingDirection(Player::EFacingDirection_Down)
		, mTimeInState(0.0f)
		, mDamagedFlashTimeRemaining(0.0f)
		, mLastSafePosition(0, 0)
	{
	}

	~PlayerUpdateState() { }

	Player::EState				mLastState;
	Player::EFacingDirection	mLastFacingDirection;
	float						mTimeInState;

	float						mDamagedFlashTimeRemaining;

	IVec2						mLastSafePosition;
};
