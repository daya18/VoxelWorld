#pragma once

namespace vw
{
	class Window
	{
	public:
		Window ( std::string const & title, glm::uvec2 const & size );
		Window ( Window const & ) = delete;
		Window ( Window && ) noexcept;
		~Window ();

		Window & operator = ( Window const & ) = delete;
		Window & operator = ( Window && ) noexcept;

		glm::uvec2 GetSize () const;
		bool ShouldClose () const;
		void MakeGLContextCurrent () const;
		void SwapBuffers () const;

	private:
		static int instanceCount;
		GLFWwindow * window { nullptr };
	};

	// Implementation
	inline bool Window::ShouldClose () const { return glfwWindowShouldClose ( window ); }
	inline void Window::MakeGLContextCurrent () const { glfwMakeContextCurrent ( window ); }
	inline void Window::SwapBuffers () const { return glfwSwapBuffers ( window ); }
}