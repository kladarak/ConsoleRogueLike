#pragma once

#include <vector>

#include <Maths/IVec2.h>
#include <GameEntities/Player/PlayerEnums.h>
#include <GameEntities/Player/Items/Inventory.h>

class PlayerComponent
{
public:
	PlayerComponent(Player::EFacingDirection inDirection) 
		: mFacingDirection			(inDirection)
		, mState					(Player::EState_Idle)
		, mIntendedMovement			(0, 0)
		, mDamaged					(false)
		, mItemSlot1				(nullptr)
		, mItemSlot2				(nullptr)
	{
	}

	PlayerComponent(PlayerComponent&& inRHS)
		: mFacingDirection			(inRHS.mFacingDirection)
		, mState					(inRHS.mState)
		, mIntendedMovement			(inRHS.mIntendedMovement)
		, mDamaged					(inRHS.mDamaged)
		, mInventory				(std::move(inRHS.mInventory))
		, mItemSlot1				(inRHS.mItemSlot1)
		, mItemSlot2				(inRHS.mItemSlot2)
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
	
	Inventory&					GetInventory()													{ return mInventory; }
	void						SetItemInSlot1(ItemBase* inItem)								{ mItemSlot1 = inItem; }
	ItemBase*					GetItemInSlot1() const											{ return mItemSlot1; }
	void						SetItemInSlot2(ItemBase* inItem)								{ mItemSlot2 = inItem; }
	ItemBase*					GetItemInSlot2() const											{ return mItemSlot2; }

private:
	Player::EFacingDirection	mFacingDirection;
	Player::EState				mState;
	IVec2						mIntendedMovement;
	bool						mDamaged;
	
	Inventory					mInventory;
	ItemBase*					mItemSlot1;
	ItemBase*					mItemSlot2;
};
