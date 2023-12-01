#pragma once

namespace vw
{
	class Window
	{
	public:
		using KeyCallback = std::function <void ( int )>;
		using ButtonCallback = std::function <void ( int )>;
		using FramebufferResizeCallback = std::function <void ( glm::vec2 const & )>;
		using ScrollCallback = std::function <void ( glm::vec2 const & delta )>;

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
		
		void InitImgui () const;
		void SetRawMouseInput ( bool enabled );

		// Polling based event handling
		bool GetKey ( int key );
		bool GetButton ( int button );
		glm::vec2 const & GetMouseDelta () const;

		// Callback based event handling
		void AddKeyDownCallback ( KeyCallback );
		void AddKeyUpCallback ( KeyCallback );
		void AddButtonDownCallback ( ButtonCallback );
		void AddButtonUpCallback ( ButtonCallback );
		void AddFramebufferResizeCallback ( FramebufferResizeCallback );
		void AddScrollCallback ( ScrollCallback );

	private:
		static Window * GetWindow ( GLFWwindow * );
		static void mouseEnterCallback ( GLFWwindow * window, int entered );
		static void mouseMoveCallback ( GLFWwindow * window, double xpos, double ypos );
		static void framebufferResizeCallback ( GLFWwindow * window, int width, int height );
		static void keyCallback ( GLFWwindow * window, int key, int scancode, int action, int mods );
		static void mouseButtonCallback ( GLFWwindow * window, int button, int action, int mods );
		static void scrollCallback ( GLFWwindow * window, double xoffset, double yoffset );

		static int instanceCount;
		GLFWwindow * window { nullptr };
		int lastKey;
		glm::vec2 lastKnownMousePosition { std::numeric_limits <float>::max (), std::numeric_limits <float>::max () };
		glm::vec2 mouseDelta { 0.0f, 0.0f };
		
		std::vector <KeyCallback> keyDownCallbacks;
		std::vector <KeyCallback> keyUpCallbacks;
		std::vector <ButtonCallback> buttonDownCallbacks;
		std::vector <ButtonCallback> buttonUpCallbacks;
		std::vector <FramebufferResizeCallback> framebufferResizeCallbacks;
		std::vector <ScrollCallback> scrollCallbacks;
	};



	// Implementation
	inline bool Window::ShouldClose () const { return glfwWindowShouldClose ( window ); }
	inline void Window::MakeGLContextCurrent () const { glfwMakeContextCurrent ( window ); }
	inline void Window::SwapBuffers () const { return glfwSwapBuffers ( window ); }
	inline glm::vec2 const & Window::GetMouseDelta () const { return mouseDelta; }
	inline void Window::AddKeyDownCallback ( KeyCallback callback )							{ keyDownCallbacks.push_back ( callback ); }
	inline void Window::AddKeyUpCallback ( KeyCallback callback )							{ keyUpCallbacks.push_back ( callback ); }
	inline void Window::AddButtonDownCallback ( ButtonCallback callback )					{ buttonDownCallbacks.push_back ( callback ); }
	inline void Window::AddButtonUpCallback ( ButtonCallback callback )						{ buttonUpCallbacks.push_back ( callback ); }
	inline void Window::AddFramebufferResizeCallback ( FramebufferResizeCallback callback ) { framebufferResizeCallbacks.push_back ( callback ); }
	inline void Window::AddScrollCallback ( ScrollCallback callback )								{ scrollCallbacks.push_back ( callback ); }
}