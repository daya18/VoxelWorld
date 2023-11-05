#include "Renderer.hpp"

#include "Window.hpp"

namespace vw
{
	Renderer::Renderer ( Window & window )
	{
		window.MakeGLContextCurrent ();
		gladLoadGL ();
		
		glEnable ( GL_DEPTH_TEST );
	}

	void Renderer::Render ()
	{
		glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		voxelRenderer.Render ();
	}
}