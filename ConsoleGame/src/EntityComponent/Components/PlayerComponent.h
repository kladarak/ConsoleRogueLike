#pragma once

#include <vector>

#include <Maths/IVec2.h>
#include <GameEntities/Player/PlayerEnums.h>
#include <GameEntities/Player/Weapons/Inventory.h>

class PlayerComponent
{
public:
	PlayerComponent(Player::EFacingDirection inDirection) 
		: mFacingDirection			(inDirection)
		, mState					(Player::EState_Idle)
		, mIntendedMovement			(0, 0)
		, mDamaged					(false)
		, mStartedAttackThisFrame	(false)
		, mSelectedWeapon			(EWeapon_Sword)
	{
	}

	PlayerComponent(PlayerComponent&& inRHS)
		: mFacingDirection			(inRHS.mFacingDirection)
		, mState					(inRHS.mState)
		, mIntendedMovement			(inRHS.mIntendedMovement)
		, mDamaged					(inRHS.mDamaged)
		, mStartedAttackThisFrame	(inRHS.mStartedAttackThisFrame)
		, mInventory				(std::move(inRHS.mInventory))
		, mSelectedWeapon			(inRHS.mSelectedWeapon)
	{
	}

	~PlayerComponent() { }
	
	Player::EFacingDirection	GetFacingDirection() const										{ return mFacingDirection; }
	void						SetFacingDirection(Player::EFacingDirection inFacingDirection)	{ mFacingDirection = inFacingDirection; }

	Player::EState				GetState() const												{ return mState; }
	void						SetState(Player::EState inState) 								{ mState = inState; }
	void						SetStartedAttackThisFrame(bool inValue)							{ mStartedAttackThisFrame = inValue; }
	bool						GetStartedAttackThisFrame() const								{ return mStartedAttackThisFrame; }

	void						SetIntendedMovement(const IVec2& inMovement)					{ mIntendedMovement = inMovement; }
	const IVec2&				GetIntendedMovement() const										{ return mIntendedMovement; }

	void						SetDamaged(bool inValue)										{ mDamaged = inValue; }
	bool						IsDamaged() const												{ return mDamaged; }
	
	const Inventory&			GetInventory() const											{ return mInventory; }
	void						AddWeapon(Weapon* inWeapon)										{ mInventory.AddWeapon(inWeapon); }
	void						SetSelectedWeapon(EWeapon inWeapon)								{ mSelectedWeapon = inWeapon; }
	Weapon*						GetSelectedWeapon() const										{ return mInventory.GetWeapon(mSelectedWeapon); }

private:
	Player::EFacingDirection	mFacingDirection;
	Player::EState				mState;
	IVec2						mIntendedMovement;
	bool						mDamaged;
	bool						mStartedAttackThisFrame;
	
	Inventory					mInventory;
	EWeapon						mSelectedWeapon;
};
