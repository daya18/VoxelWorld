#pragma once

#include "VoxelRenderer.hpp"

namespace vw
{
	class Window;
	class Camera;

	class Renderer
	{
	public:
		Renderer ( Window & window );
		
		void SetCamera ( Camera & camera );

		void Render ();

		VoxelRenderer & GetVoxelRenderer () const;

	private:
		static void GLDebugMessageCallback (
			GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * message, const void * userParam );

		// It is a pointer to prevent implicit default initialization before opengl loading
		std::unique_ptr <VoxelRenderer> voxelRenderer;
	};

	inline VoxelRenderer & Renderer::GetVoxelRenderer () const { return *voxelRenderer; }
}