#include "Player.hpp"

#include "Window.hpp"
#include "Application.hpp"
#include "World.hpp"

namespace vw
{
	float const Player::blockPlaceBreakDelay { 0.2f };
	
	Player::Player ( World & world )
		: world ( & world )
	{
	}

	void Player::Update ( float deltaTime )
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

					if ( raycaster.GetTargetVoxel () != nullptr  )
					{ 
						auto position { raycaster.GetTargetVoxel ()->GetPosition () 
							+ GetDirectionVector ( raycaster.GetTargetVoxelSide () ) };

						voxelWorld.AddVoxel ( position, "Log" );
					}
				}
				nextBlockPlaceBreakCountdown -= deltaTime;
			}
		}

		if ( ! window.GetButton ( GLFW_MOUSE_BUTTON_LEFT ) && !window.GetButton ( GLFW_MOUSE_BUTTON_RIGHT ) )
			nextBlockPlaceBreakCountdown = 0.0f;
	}

	void Player::Render ()
	{

	}
}