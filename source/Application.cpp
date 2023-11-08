#include "Application.hpp"

namespace vw
{
	Application::Application ()
	: 
		window { "Voxel World", { 1280, 720 } },
		renderer ( window ),
		
		projectionMatrix ( glm::perspective ( 
			45.0f, static_cast <float> ( 1280 ) / static_cast <float> ( 720 ), 0.001f, 100.0f ) ),

		world ( window, renderer )
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

			world.Update ( 1.0f );

			window.MakeGLContextCurrent ();
			renderer.Render ();
			window.SwapBuffers ();
		}
	}
}