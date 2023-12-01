#pragma once

namespace vw
{
	class Inventory
	{
	public:
		struct ItemStack
		{
			std::string itemType {};
			int itemCount {};
		};

		class ItemSlot
		{
		public:
			void PlaceItemStack ( ItemStack const & );
			ItemStack const & GetItemStack () const;
			ItemStack TakeItemStack ();
			bool CheckEmpty () const;

		private:
			bool isEmpty { true };
			ItemStack itemStack {};
		};

		Inventory ( int size );

		ItemSlot & GetSlot ( int index );

	private:
		std::vector <ItemSlot> slots;
	};
}