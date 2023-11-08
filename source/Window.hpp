#pragma once

#include "SignalEmitter.hpp"

namespace vw
{
	class Window : public SignalEmitter
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
		void HandleInput ();

		int GetLastKey () const;
		bool GetKey ( int key );

		void EnableRawMouseInput ();
		void DisableRawMouseInput ();

		glm::vec2 const & GetMouseDelta () const;

	private:
		static Window * GetWindow ( GLFWwindow * );
		static void mouseEnterCallback ( GLFWwindow * window, int entered );
		static void mouseMoveCallback ( GLFWwindow * window, double xpos, double ypos );
		static void framebufferResizeCallback ( GLFWwindow * window, int width, int height );
		static void keyCallback ( GLFWwindow * window, int key, int scancode, int action, int mods );

		static int instanceCount;
		GLFWwindow * window { nullptr };
		int lastKey;
		glm::vec2 lastKnownMousePosition {};
		glm::vec2 mouseDelta {};
	};

	// Implementation
	inline bool Window::ShouldClose () const { return glfwWindowShouldClose ( window ); }
	inline void Window::MakeGLContextCurrent () const { glfwMakeContextCurrent ( window ); }
	inline void Window::SwapBuffers () const { return glfwSwapBuffers ( window ); }
	inline int Window::GetLastKey () const { return lastKey; }
	inline glm::vec2 const & Window::GetMouseDelta () const { return mouseDelta; }

}