#pragma once

class PlayerComponent
{
public:
	enum EFacingDirection
	{
		EFacingDirection_Up,
		EFacingDirection_Down,
		EFacingDirection_Left,
		EFacingDirection_Right,
	};

	enum EState
	{
		EState_Idle,
		EState_Attacking,
		EState_Defending,
	};

	PlayerComponent(EFacingDirection inDirection) : mFacingDirection(inDirection), mState(EState_Idle) { }
	~PlayerComponent() { }
	
	EFacingDirection	GetFacingDirection() const								{ return mFacingDirection; }
	void				SetFacingDirection(EFacingDirection inFacingDirection)	{ mFacingDirection = inFacingDirection; }

	EState				GetState() const										{ return mState; }
	void				SetState(EState inState) 								{ mState = inState; }

private:
	EFacingDirection	mFacingDirection;
	EState				mState;

};
