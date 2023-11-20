#include "Application.hpp"

#include "TestVoxelWorld.hpp"
#include "Utility.hpp"

namespace vw
{
	Application::Application ()
	: 
		window { "Voxel World", { 1280, 720 } },
		renderer ( window ),
		
		voxelTypeTextures {
			{ "Grass", CreateTextureFromFile ( "image/TestBlockTexture.png" ) }
		},

		world ( std::make_unique <TestVoxelWorld> ( *this ) )
	{
		window.EnableRawMouseInput ();
	}

	void Application::Run ()
	{
		while ( ! quit )
		{
			glfwPollEvents ();
			
			if ( window.ShouldClose () )
			{
				quit = true;
				break;
			}

			window.HandleInput ();

			world->Update ( 1.0f );

			renderer.Begin ();
			world->Render ();
			renderer.End ();

		}
	}
}