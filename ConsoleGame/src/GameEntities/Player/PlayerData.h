#pragma once

#include <Inventory/Inventory.h>

#include "PlayerEnums.h"

class PlayerData
{
public:
	PlayerData() { memset(mItemSlot, 0, sizeof(mItemSlot)); mMaxHealth = 3; }
	
	void		SetItemInSlot(ItemBase* inItem, Player::EItemSlot inSlot)	{ if (IsItemSlotInRange(inSlot)) { mItemSlot[inSlot] = inItem; } }
	ItemBase*	GetItemInSlot(Player::EItemSlot inSlot) const				{ return IsItemSlotInRange(inSlot) ? mItemSlot[inSlot] : nullptr; }

	Inventory	mInventory;
	int			mMaxHealth;

private:
	static bool	IsItemSlotInRange(Player::EItemSlot inSlot)					{ return (inSlot >= Player::EItemSlot_Slot0 && inSlot < Player::EItemSlot_SlotCount); }
	
	ItemBase*	mItemSlot[Player::EItemSlot_SlotCount];

};
