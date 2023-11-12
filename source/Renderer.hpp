#pragma once

#include "VoxelWorldRenderer.hpp"

namespace vw
{
	class Window;
	class Camera;

	class Renderer
	{
	public:
		Renderer ( Window & window );
		
		void Begin ();
		void End ();

	private:
		static void GLDebugMessageCallback (
			GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * message, const void * userParam );

		Window * window;
	};
}