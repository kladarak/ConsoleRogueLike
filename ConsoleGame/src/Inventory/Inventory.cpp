#include "Inventory.h"

#include "Items/ItemBase.h"

Inventory::~Inventory()
{
	for (auto item : mItems)
	{
		delete item;
	}

	mItems.clear();
}

void Inventory::AddItem(ItemBase* inItem)
{
	mItems.push_back(inItem);
}
