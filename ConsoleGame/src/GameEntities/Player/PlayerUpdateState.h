#pragma once

#include "PlayerEnums.h"

class PlayerUpdateState
{
public:
	PlayerUpdateState()
		: mLastFacingDirection(Player::EFacingDirection_Down)
		, mDamagedFlashTimeRemaining(0.0f)
		, mLastSafePosition(0, 0)
	{
	}

	~PlayerUpdateState() { }

	Player::EFacingDirection	mLastFacingDirection;
	float						mDamagedFlashTimeRemaining;
	IVec2						mLastSafePosition;
};
