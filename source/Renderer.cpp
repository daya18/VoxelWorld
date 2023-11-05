#include "Renderer.hpp"

#include "Window.hpp"

namespace vw
{
	Renderer::Renderer ( Window & window )
	{
		window.MakeGLContextCurrent ();
		gladLoadGL ();
		
		glDebugMessageCallback ( GLDebugMessageCallback, nullptr );

		glEnable ( GL_DEBUG_OUTPUT );
		glEnable ( GL_DEPTH_TEST );

		voxelRenderer = std::make_unique <VoxelRenderer> ();
	}

	void Renderer::Render ()
	{
		glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		voxelRenderer->Render ();
	}

	void Renderer::GLDebugMessageCallback ( 
		GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * message, const void * userParam ) 
	{
		// You can customize the behavior of the callback here
		// For example, you can filter messages based on severity, type, or source
		if ( severity == GL_DEBUG_SEVERITY_HIGH || severity == GL_DEBUG_SEVERITY_MEDIUM ) {
			std::cerr << "OpenGL Debug Message: " << message << std::endl;
		}
	}
}