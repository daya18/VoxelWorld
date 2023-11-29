#pragma once

#include "Window.hpp"
#include "RenderContext.hpp"
#include "World.hpp"

namespace vw
{
	class Application
	{
	public:
		Application ();
		Application ( Application const & ) = delete;
		Application ( Application && ) = delete;

		~Application ();

		Application & operator = ( Application const & ) = delete;
		Application & operator = ( Application && ) = delete;

		void Run ();
		Window & GetWindow ();

	private:
		void SetPaused ( bool paused );
		void Update ( float deltaTime );
		void Render ();

		bool quit { false };
		bool paused;
		Window window;
		RenderContext renderContext;
		std::unordered_map <std::string, GLuint> voxelTypeTextures;
		World world;
		std::chrono::steady_clock::time_point lastFrameTime;

		friend class VoxelWorld;
	};

	// Implementation
	inline Window & Application::GetWindow () { return window; }
}