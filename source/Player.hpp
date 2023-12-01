#pragma once

#include "Inventory.hpp"
#include "PlayerInventoryHUD.hpp"

namespace vw
{
	class World;
	class GUIRenderer;

	class Player
	{
	public:
		Player ( World & );

		void Update ( float deltaTime );
		void RenderGUI ( GUIRenderer & );

	private:
		void HandleBuild ( float deltaTime );

		static float const blockPlaceBreakDelay;

		World * world;
		float nextBlockPlaceBreakCountdown { 0.0f };
		Inventory inventory;
		PlayerInventoryHUD inventoryHUD;
	};
}