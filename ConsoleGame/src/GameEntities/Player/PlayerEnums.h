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
		EState_StartUsingItem1,
		EState_UsingItem1,
		EState_StartUsingItem2,
		EState_UsingItem2,
	};
}
