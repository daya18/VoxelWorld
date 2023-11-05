#include "Application.hpp"

namespace vw
{
	Application::Application ()
	: 
		window { "Voxel World", { 1280, 720 } },
		renderer ( window )
	{
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

			window.MakeGLContextCurrent ();
			renderer.Render ();
			window.SwapBuffers ();
		}
	}
}