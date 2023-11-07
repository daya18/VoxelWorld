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
		GetWindow(glfwWindow)->EmitSignal ("MouseMove");
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

	bool Window::GetKey ( int key )
	{
		return glfwGetKey ( window, key );
	}
}