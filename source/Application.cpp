#include "Application.hpp"

namespace vw
{
	Application::Application ()
	{
		glfwInit ();
		window = Window { "Voxel World", { 1280, 720 } };
	}

	Application::~Application ()
	{
		glfwTerminate ();
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
			glClear ( GL_COLOR_BUFFER_BIT );
			window.SwapBuffers ();
		}
	}
}