#pragma once

#include "gui/Widgets.hpp"

namespace vw
{
	class Application;
	class GUIRenderer;
	class Inventory;

	class PlayerInventoryHUD
	{
	public:
		PlayerInventoryHUD ( Application &, Inventory & );

		void Update ( float deltaTime );
		void Render ( GUIRenderer & );

		int GetSelectedSlotIndex () const;

	private:
		Application * application;
		Inventory * inventory;
		int selectedSlotIndex { 0 };
	};
}