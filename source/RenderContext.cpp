#include "RenderContext.hpp"

#include "Window.hpp"

#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

namespace vw
{
	RenderContext::RenderContext ( Window & window )
		: window ( & window )
	{
		window.MakeGLContextCurrent ();
		gladLoadGL ();

		glDebugMessageCallback ( GLDebugMessageCallback, nullptr );

		glEnable ( GL_DEBUG_OUTPUT );
		glEnable ( GL_DEPTH_TEST );

		imguiContext = ImGui::CreateContext ();

		window.InitImgui ();
		ImGui_ImplOpenGL3_Init ();
	}

	RenderContext::~RenderContext ()
	{
		ImGui_ImplGlfw_Shutdown ();
		ImGui_ImplOpenGL3_Shutdown ();
		ImGui::DestroyContext ( imguiContext );
	}

	void RenderContext::Begin ()
	{
		window->MakeGLContextCurrent ();
		auto windowSize { window->GetSize () };
		glViewport ( 0, 0, windowSize.x, windowSize.y );
		glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		ImGui_ImplGlfw_NewFrame ();
		ImGui_ImplOpenGL3_NewFrame ();
		ImGui::NewFrame ();
	}

	void RenderContext::End ()
	{
		ImGui::EndFrame ();
		ImGui::Render ();
		ImGui_ImplOpenGL3_RenderDrawData ( ImGui::GetDrawData () );

		window->SwapBuffers ();
	}

	void RenderContext::GLDebugMessageCallback (
		GLenum source, GLenum type, GLuint id, GLenum severity, 
		GLsizei length, const GLchar * message, const void * userParam ) 
	{
		// You can customize the behavior of the callback here
		// For example, you can filter messages based on severity, type, or source
		if ( severity == GL_DEBUG_SEVERITY_HIGH || severity == GL_DEBUG_SEVERITY_MEDIUM ) {
			std::cerr << "OpenGL Debug Message: " << message << std::endl;
		}
	}
}