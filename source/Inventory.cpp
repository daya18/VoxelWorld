#include "Inventory.hpp"

namespace vw
{
	void Inventory::ItemSlot::PlaceItemStack ( ItemStack const & itemStack )
	{
		isEmpty = false;
		this->itemStack = itemStack;
	}
	
	Inventory::ItemStack const & Inventory::ItemSlot::GetItemStack () const
	{
		return itemStack;
	}

	Inventory::ItemStack Inventory::ItemSlot::TakeItemStack ()
	{
		isEmpty = true;
		return std::exchange ( this->itemStack, ItemStack {} );
	}

	bool Inventory::ItemSlot::CheckEmpty () const
	{
		return isEmpty;
	}

	Inventory::Inventory ( int size )
	{
		slots.resize ( size );
	}

	Inventory::ItemSlot & Inventory::GetSlot ( int index )
	{
		return slots.at ( index );
	}
}