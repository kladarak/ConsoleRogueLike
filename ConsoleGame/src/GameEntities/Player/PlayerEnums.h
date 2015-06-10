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
		EState_Attacking,
		EState_Defending,
	};
}
