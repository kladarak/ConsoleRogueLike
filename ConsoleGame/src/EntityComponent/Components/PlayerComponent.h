#pragma once

#include <vector>

#include <EntityComponentSystem/World/World.h>

#include <Maths/IVec2.h>
#include <GameEntities/Player/PlayerEnums.h>
#include <GameEntities/Player/Items/Inventory.h>

class AttackMsg;
class InputBuffer;
class PlayerBehaviourBase;
class MessageBroadcaster;

class PlayerComponent
{
public:
	PlayerComponent();
	PlayerComponent(PlayerComponent&& inRHS);

	~PlayerComponent();
	
	void						HandleInput(const InputBuffer& inBuffer);
	void						Update(Entity inPlayer, float inFrameTime);
	void						OnAttacked(Entity inPlayer, const AttackMsg& inAttackMsg, MessageBroadcaster& inBroadcaster);

	Player::EItemSlot			GetUsingItemSlot() const										{ return mUsingItemSlot; }
	void						SetUsingItemSlot(Player::EItemSlot inSlot) 						{ mUsingItemSlot = inSlot; }
	ItemBase*					GetUsingItem() const											{ return GetItemInSlot(mUsingItemSlot); }

	Inventory&					GetInventory()													{ return mInventory; }
	void						SetItemInSlot(ItemBase* inItem, Player::EItemSlot inSlot)		{ if (IsItemSlotInRange(inSlot)) { mItemSlot[inSlot] = inItem; } }
	ItemBase*					GetItemInSlot(Player::EItemSlot inSlot) const					{ return IsItemSlotInRange(inSlot) ? mItemSlot[inSlot] : nullptr; }

private:
	static bool					IsItemSlotInRange(Player::EItemSlot inSlot)						{ return (inSlot >= Player::EItemSlot_Slot0 && inSlot < Player::EItemSlot_SlotCount); }
	
	void						UpdateState(Entity inPlayer);
	void						UpdateOrientation(Entity inPlayer);
	void						UpdatePosition(Entity inPlayer);
	void						UpdateBehaviour(Entity inPlayer, float inFrameTime);
	void						UpdateAnimation(Entity inPlayer, float inFrameTime);
	
	enum EState
	{
		EState_Idle,
		EState_UseItem,
		EState_Dead
	};
	
	struct Intention
	{
		IVec2				mMovement;
		EOrientation		mFacingDirection;
		EState				mState;
		Player::EItemSlot	mUseItemSlot;

		Intention() 
			: mMovement			(0, 0)
			, mFacingDirection	(EOrientation_Count)
			, mState			(EState_Idle)
			, mUseItemSlot		(Player::EItemSlot_None)
		{
		}
	};

	Intention					mIntention;

	EState						mState;
	Player::EItemSlot			mUsingItemSlot;
	
	PlayerBehaviourBase*		mIdleBehaviour;
	PlayerBehaviourBase*		mDeadBehaviour;
	PlayerBehaviourBase*		mCurrentBehaviour;

	IVec2						mLastSafePosition;
	float						mDamagedFlashTimeRemaining;
	
	Inventory					mInventory;
	ItemBase*					mItemSlot[Player::EItemSlot_SlotCount];

};
