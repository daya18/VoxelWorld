#pragma once

#include "gui/Widgets.hpp"

namespace vw
{
	class Application;
	class GUIRenderer;
	class Inventory;

	class InventoryView
	{
	public:
		InventoryView ( Application &, Inventory & );

		void Update ( float deltaTime );
		void Render ( GUIRenderer & );

	private:
		Application * application;
		Inventory * inventory;
	};
}