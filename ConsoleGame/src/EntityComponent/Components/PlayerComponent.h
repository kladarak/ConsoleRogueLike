#pragma once

#include <vector>

#include <Maths/IVec2.h>
#include <GameEntities/Player/PlayerEnums.h>
#include <GameEntities/Player/Items/Inventory.h>

class PlayerComponent
{
public:
	PlayerComponent(Player::EFacingDirection inDirection) 
		: mFacingDirection	(inDirection)
		, mState			(Player::EState_Idle)
		, mDamaged			(false)
	{
		memset(mItemSlot, 0, sizeof(mItemSlot));
	}

	PlayerComponent(PlayerComponent&& inRHS)
		: mIntention		(inRHS.mIntention)
		, mFacingDirection	(inRHS.mFacingDirection)
		, mState			(inRHS.mState)
		, mDamaged			(inRHS.mDamaged)
		, mInventory		(std::move(inRHS.mInventory))
	{
		memcpy(mItemSlot, inRHS.mItemSlot, sizeof(mItemSlot));
	}

	~PlayerComponent() { }

	void						SetIntention(const Player::Intention& inIntention)				{ mIntention = inIntention; }
	const Player::Intention&	GetIntention() const											{ return mIntention; }

	Player::EFacingDirection	GetFacingDirection() const										{ return mFacingDirection; }
	void						SetFacingDirection(Player::EFacingDirection inFacingDirection)	{ mFacingDirection = inFacingDirection; }

	Player::EState				GetState() const												{ return mState; }
	void						SetState(Player::EState inState) 								{ mState = inState; }

	Player::EItemSlot			GetUsingItemSlot() const										{ return mUsingItemSlot; }
	void						SetUsingItemSlot(Player::EItemSlot inSlot) 						{ mUsingItemSlot = inSlot; }
	ItemBase*					GetUsingItem() const											{ return GetItemInSlot(mUsingItemSlot); }

	void						SetDamaged(bool inValue)										{ mDamaged = inValue; }
	bool						IsDamaged() const												{ return mDamaged; }
	
	Inventory&					GetInventory()													{ return mInventory; }
	void						SetItemInSlot(ItemBase* inItem, Player::EItemSlot inSlot)		{ if (IsItemSlotInRage(inSlot)) { mItemSlot[inSlot] = inItem; } }
	ItemBase*					GetItemInSlot(Player::EItemSlot inSlot) const					{ return IsItemSlotInRage(inSlot) ? mItemSlot[inSlot] : nullptr; }

private:
	static bool					IsItemSlotInRage(Player::EItemSlot inSlot)						{ return (inSlot >= Player::EItemSlot_Slot0 && inSlot < Player::EItemSlot_SlotCount); }
	
	Player::Intention			mIntention;

	Player::EFacingDirection	mFacingDirection;
	Player::EState				mState;
	Player::EItemSlot			mUsingItemSlot;
	bool						mDamaged;
	
	Inventory					mInventory;
	ItemBase*					mItemSlot[Player::EItemSlot_SlotCount];
};
