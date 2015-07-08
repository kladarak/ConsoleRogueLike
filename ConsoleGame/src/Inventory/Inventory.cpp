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

ItemBase* Inventory::FindItem(const std::string& inName) const
{
	for (auto item : mItems)
	{
		if (item->GetName() == inName)
		{
			return item;
		}
	}

	return nullptr;
}

void Inventory::RemoveAndDeleteItem(ItemBase* inItem)
{
	for (auto iter = mItems.begin(), end = mItems.end(); iter != end; ++iter)
	{
		if ((*iter) == inItem)
		{
			mItems.erase(iter);
			delete inItem;
			break;
		}
	}
}

