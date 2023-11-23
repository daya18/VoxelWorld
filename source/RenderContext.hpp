#pragma once

namespace vw
{
	class Window;
	class Camera;

	class RenderContext
	{
	public:
		RenderContext ( Window & window );
		~RenderContext ();

		void Begin ();
		void End ();

	private:
		static void GLDebugMessageCallback (
			GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * message, const void * userParam );

		Window * window;
		ImGuiContext * imguiContext;
	};
}