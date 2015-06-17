#pragma once

#include <vector>

#include <EntityComponentSystem/World/World.h>

#include <Maths/IVec2.h>
#include <GameEntities/Player/PlayerEnums.h>
#include <GameEntities/Player/Items/Inventory.h>

class AttackMsg;
class InputBuffer;
class PlayerBehaviourBase;

class PlayerComponent
{
public:
	PlayerComponent(Player::EFacingDirection inDirection);
	PlayerComponent(PlayerComponent&& inRHS);

	~PlayerComponent();
	
	void						HandleInput(const InputBuffer& inBuffer);
	void						Update(Entity inPlayer, float inFrameTime);
	void						OnAttacked(Entity inPlayer, const AttackMsg& inAttackMsg);

	Player::EFacingDirection	GetFacingDirection() const										{ return mFacingDirection; }
	Player::EState				GetState() const												{ return mState; }

	Player::EItemSlot			GetUsingItemSlot() const										{ return mUsingItemSlot; }
	void						SetUsingItemSlot(Player::EItemSlot inSlot) 						{ mUsingItemSlot = inSlot; }
	ItemBase*					GetUsingItem() const											{ return GetItemInSlot(mUsingItemSlot); }

	Inventory&					GetInventory()													{ return mInventory; }
	void						SetItemInSlot(ItemBase* inItem, Player::EItemSlot inSlot)		{ if (IsItemSlotInRange(inSlot)) { mItemSlot[inSlot] = inItem; } }
	ItemBase*					GetItemInSlot(Player::EItemSlot inSlot) const					{ return IsItemSlotInRange(inSlot) ? mItemSlot[inSlot] : nullptr; }

private:
	static bool					IsItemSlotInRange(Player::EItemSlot inSlot)						{ return (inSlot >= Player::EItemSlot_Slot0 && inSlot < Player::EItemSlot_SlotCount); }
	
	void TakeDamage(Entity inPlayer);
	void CheckAndHandleIfInDamageZone(Entity inPlayer);
	void UpdatePosition(Entity inPlayer);
	void UpdateState(Entity inPlayer, float inFrameTime);
	void UpdateAnimation(Entity inPlayer, float inFrameTime);

	struct Intention
	{
		IVec2						mMovement;
		Player::EFacingDirection	mFacingDirection;
		Player::EState				mState;
		Player::EItemSlot			mUseItemSlot;

		Intention() 
			: mMovement			(0, 0)
			, mFacingDirection	(Player::EFacingDirection_Count)
			, mState			(Player::EState_Idle)
			, mUseItemSlot		(Player::EItemSlot_None)
		{
		}
	};

	Intention					mIntention;

	Player::EFacingDirection	mFacingDirection;
	Player::EState				mState;
	Player::EItemSlot			mUsingItemSlot;
	PlayerBehaviourBase*		mCurrentBehaviour;

	Player::EFacingDirection	mLastFacingDirection;
	float						mDamagedFlashTimeRemaining;
	IVec2						mLastSafePosition;
	
	Inventory					mInventory;
	ItemBase*					mItemSlot[Player::EItemSlot_SlotCount];

};
