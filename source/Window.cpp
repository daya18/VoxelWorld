#include "Window.hpp"

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
		GetWindow ( glfwWindow )->EmitSignal ( "FramebufferResize" );
	}

	void Window::keyCallback ( GLFWwindow * glfwWindow, int key, int scancode, int action, int mods )
	{
		GetWindow ( glfwWindow )->lastKey = key;

		if ( action == GLFW_PRESS )
			GetWindow ( glfwWindow )->EmitSignal ( "KeyPress" );
		else
			GetWindow ( glfwWindow )->EmitSignal ( "KeyRelease" );
	}

	void Window::mouseButtonCallback ( GLFWwindow * window, int button, int action, int mods )
	{

	}

	void Window::mouseEnterCallback ( GLFWwindow * glfwWindow, int entered )
	{
	/*	if ( entered )
		{
			double x, y;
			glfwGetCursorPos ( glfwWindow, &x, &y );
			GetWindow ( glfwWindow )->lastKnownMousePosition = glm::vec2 { x, y };
			GetWindow ( glfwWindow )->mouseDelta = { 0.0f, 0.0f };
		}
	*/
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

	void Window::EnableRawMouseInput ()
	{
		if ( glfwRawMouseMotionSupported () )
		{
			glfwSetInputMode ( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
			glfwSetInputMode ( window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE );
		}
	}

	void Window::DisableRawMouseInput ()
	{
		if ( glfwRawMouseMotionSupported () )
		{
			glfwSetInputMode ( window, GLFW_CURSOR, GLFW_CURSOR_NORMAL );
			glfwSetInputMode ( window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE );
		}
	}

	void Window::InitImgui () const 
	{
		ImGui_ImplGlfw_InitForOpenGL ( window, true );
	}
}