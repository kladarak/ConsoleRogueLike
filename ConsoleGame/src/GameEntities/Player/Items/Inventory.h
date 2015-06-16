#pragma once

#include <vector>
#include <Core/Debug/Assert.h>

class ItemBase;

class Inventory
{
public:
	Inventory()																{ }
	Inventory(Inventory&& inRHS)		: mItems( std::move(inRHS.mItems) )	{ }
	Inventory(const Inventory& inRHS)	: mItems( inRHS.mItems )			{ assert(false); } // Watch out! TODO: Do a deep copy...
	~Inventory();

	void							AddItem(ItemBase* inItem);
	const std::vector<ItemBase*>&	GetAllItems() const						{ return mItems; }

private:
	std::vector<ItemBase*>			mItems;

};
