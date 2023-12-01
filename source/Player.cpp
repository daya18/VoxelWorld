#include "Player.hpp"

#include "Window.hpp"
#include "Application.hpp"
#include "World.hpp"

namespace vw
{
	float const Player::blockPlaceBreakDelay { 0.2f };
	
	Player::Player ( World & world )
		:
		world ( & world ),
		inventory ( 6 * 4 ),
		inventoryHUD ( world.GetApplication (), inventory )
	{
		inventory.GetSlot ( 0 ).PlaceItemStack ( { "Grass", 10 } );
		inventory.GetSlot ( 1 ).PlaceItemStack ( { "Stone", 10 } );
		inventory.GetSlot ( 2 ).PlaceItemStack ( { "Plank", 10 } );
	}

	void Player::Update ( float deltaTime )
	{
		if ( ! world->GetApplication ().CheckPaused () )
			HandleBuild ( deltaTime );
	}

	void Player::RenderGUI ( GUIRenderer & renderer )
	{
		inventoryHUD.Render ( renderer );
	}

	void Player::HandleBuild ( float deltaTime )
	{
		auto & window { world->GetApplication ().GetWindow () };
		auto & voxelWorld { world->GetVoxelWorld () };
		auto & raycaster { voxelWorld.GetRaycaster () };

		if ( window.GetButton ( GLFW_MOUSE_BUTTON_LEFT ) != window.GetButton ( GLFW_MOUSE_BUTTON_RIGHT ) )
		{
			if ( window.GetButton ( GLFW_MOUSE_BUTTON_LEFT ) )
			{
				if ( nextBlockPlaceBreakCountdown <= 0.0f )
				{
					nextBlockPlaceBreakCountdown = blockPlaceBreakDelay;

					if ( raycaster.GetTargetVoxel () != nullptr )
						voxelWorld.RemoveVoxel ( raycaster.GetTargetVoxel () );
				}
				else
					nextBlockPlaceBreakCountdown -= deltaTime;
			}

			if ( window.GetButton ( GLFW_MOUSE_BUTTON_RIGHT ) )
			{
				if ( nextBlockPlaceBreakCountdown <= 0.0f )
				{
					nextBlockPlaceBreakCountdown = blockPlaceBreakDelay;

					if ( raycaster.GetTargetVoxel () != nullptr )
					{
						auto position { raycaster.GetTargetVoxel ()->GetPosition ()
							+ GetDirectionVector ( raycaster.GetTargetVoxelSide () ) };

						auto & selectedSlot { inventory.GetSlot ( inventoryHUD.GetSelectedSlotIndex () ) };

						if ( ! selectedSlot.CheckEmpty () )
							voxelWorld.AddVoxel ( position, selectedSlot.GetItemStack ().itemType );
					}
				}
				nextBlockPlaceBreakCountdown -= deltaTime;
			}

		}

		if ( ! window.GetButton ( GLFW_MOUSE_BUTTON_LEFT ) && !window.GetButton ( GLFW_MOUSE_BUTTON_RIGHT ) )
			nextBlockPlaceBreakCountdown = 0.0f;
	}

}