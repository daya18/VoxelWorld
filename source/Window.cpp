#include "Window.hpp"

#include "Core.hpp"

namespace vw
{
	int Window::instanceCount { 0 };

	Window::Window ( std::string const & title, glm::uvec2 const & size )
	{
		if ( instanceCount == 0 )
			glfwInit ();

		glfwWindowHint ( GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE );
		window = glfwCreateWindow ( size.x, size.y, title.data (), nullptr, nullptr );
		glfwSetWindowUserPointer ( window, this );

		glfwSetCursorEnterCallback ( window, mouseEnterCallback );
		glfwSetCursorPosCallback ( window, mouseMoveCallback );
		glfwSetFramebufferSizeCallback ( window, framebufferResizeCallback );
		glfwSetKeyCallback ( window, keyCallback );

		++instanceCount;
	}

	Window::Window ( Window && r ) noexcept
	{
		operator = ( std::move ( r ) );
	}

	Window::~Window ()
	{
		if ( window )
			glfwDestroyWindow ( window );

		--instanceCount;

		if ( instanceCount == 0 )
			glfwTerminate ();
	}

	Window & Window::operator = ( Window && r ) noexcept
	{
		window = std::exchange ( r.window, nullptr );
		glfwSetWindowUserPointer ( window, this );
		return *this;
	}

	glm::uvec2 Window::GetSize () const
	{
		int width, height;
		glfwGetFramebufferSize ( window, &width, &height );
		return { static_cast < unsigned int > ( width ), static_cast < unsigned int > ( height ) };
	}

	Window * Window::GetWindow ( GLFWwindow * glfwWindow )
	{
		return reinterpret_cast < Window * > ( glfwGetWindowUserPointer ( glfwWindow ) );
	}

	void Window::mouseMoveCallback ( GLFWwindow * glfwWindow, double xpos, double ypos )
	{
	}

	void Window::framebufferResizeCallback ( GLFWwindow * glfwWindow, int width, int height )
	{
		auto & window { *GetWindow ( glfwWindow ) };
		Invoke ( window.framebufferResizeCallbacks, glm::vec2 { width, height } );
	}

	void Window::keyCallback ( GLFWwindow * glfwWindow, int key, int scancode, int action, int mods )
	{
		auto & window { *GetWindow ( glfwWindow ) };

		if ( action == GLFW_PRESS )
			Invoke ( window.keyDownCallbacks, key );
		else
			Invoke ( window.keyUpCallbacks, key );
	}

	void Window::mouseButtonCallback ( GLFWwindow * glfwWindow, int button, int action, int mods )
	{
		auto & window { *GetWindow ( glfwWindow ) };

		if ( action == GLFW_PRESS )
			Invoke ( window.buttonDownCallbacks, button );
		else
			Invoke ( window.buttonUpCallbacks, button );
	}

	void Window::mouseEnterCallback ( GLFWwindow * glfwWindow, int entered )
	{
	}

	bool Window::GetKey ( int key )
	{
		return glfwGetKey ( window, key );
	}
	
	bool Window::GetButton ( int button )
	{
		return glfwGetMouseButton ( window, button );
	}

	void Window::HandleInput ()
	{
		double x, y;
		glfwGetCursorPos ( window, &x, &y );
		glm::vec2 currentMousePosition { x, y };
		
		if ( lastKnownMousePosition != glm::vec2 { std::numeric_limits <float>::max (), std::numeric_limits <float>::max () } )
			mouseDelta = currentMousePosition - lastKnownMousePosition;
		
		lastKnownMousePosition = currentMousePosition;
	}

	void Window::SetRawMouseInput ( bool enabled )
	{
		if ( glfwRawMouseMotionSupported () )
		{
			glfwSetInputMode ( window, GLFW_CURSOR, enabled ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL );
			glfwSetInputMode ( window, GLFW_RAW_MOUSE_MOTION, enabled ? GLFW_TRUE : GLFW_FALSE );
		}
	}

	void Window::InitImgui () const 
	{
		ImGui_ImplGlfw_InitForOpenGL ( window, true );
	}
}