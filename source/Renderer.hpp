#pragma once

#include "VoxelRenderer.hpp"

namespace vw
{
	class Window;

	class Renderer
	{
	public:
		Renderer ( Window & window );
		
		void Render ();

	private:
		static void GLDebugMessageCallback (
			GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * message, const void * userParam );

		// It is a pointer to prevent implicit default initialization before opengl loading
		std::unique_ptr <VoxelRenderer> voxelRenderer;
	};
}