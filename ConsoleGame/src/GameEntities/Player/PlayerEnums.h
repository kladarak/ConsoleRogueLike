#pragma once

namespace Player
{
	enum EFacingDirection
	{
		EFacingDirection_Up,
		EFacingDirection_Down,
		EFacingDirection_Left,
		EFacingDirection_Right,

		EFacingDirection_Count
	};

	enum EState
	{
		EState_Idle,
		EState_UseItem,
	};

	enum EItemSlot
	{
		EItemSlot_None	= -1,
		EItemSlot_Slot0 = 0,
		EItemSlot_Slot1,

		EItemSlot_SlotCount,
	};

	struct Intention
	{
		IVec2				mMovement;
		EFacingDirection	mFacingDirection;
		EState				mState;
		EItemSlot			mUseItemSlot;

		Intention() 
			: mMovement			(0, 0)
			, mFacingDirection	(Player::EFacingDirection_Count)
			, mState			(Player::EState_Idle)
			, mUseItemSlot		(EItemSlot_None)
		{
		}
	};
}
