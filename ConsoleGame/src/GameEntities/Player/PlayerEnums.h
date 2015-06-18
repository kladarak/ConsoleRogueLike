#pragma once

#include <EntityComponent/Components/EOrientation.h>

namespace Player
{
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
		EOrientation		mFacingDirection;
		Player::EState		mState;
		Player::EItemSlot	mUseItemSlot;

		Intention() 
			: mMovement			(0, 0)
			, mFacingDirection	(EOrientation_Count)
			, mState			(Player::EState_Idle)
			, mUseItemSlot		(Player::EItemSlot_None)
		{
		}
	};
}
