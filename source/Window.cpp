#include "Window.hpp"

namespace vw
{
	Window::Window () {}

	Window::Window ( std::string const & title, glm::uvec2 const & size )
		: window ( glfwCreateWindow ( size.x, size.y, title.data (), nullptr, nullptr ) )
	{
	}

	Window::Window ( Window && r ) noexcept
	{
		operator = ( std::move ( r ) );
	}

	Window::~Window ()
	{
		if ( window )
			glfwDestroyWindow ( window );
	}

	Window & Window::operator = ( Window && r ) noexcept
	{
		window = std::exchange ( r.window, nullptr );
		return *this;
	}

	glm::uvec2 Window::GetSize () const
	{
		int width, height;
		glfwGetFramebufferSize ( window, &width, &height );
		return { static_cast < unsigned int > ( width ), static_cast < unsigned int > ( height ) };
	}
}