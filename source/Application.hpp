#pragma once

#include "Window.hpp"
#include "RenderContext.hpp"
#include "World.hpp"
#include "gui/GUIRenderer.hpp"

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
		bool CheckPaused () const;

		GLuint GetVoxelTexture ( std::string const & ) const;
		GLuint GetItemTexture ( std::string const & ) const;
		GLuint GetWhiteTexture () const;

	private:
		void SetPaused ( bool paused );
		void Update ( float deltaTime );
		void FixedUpdate ();
		void Render ();
		void RenderGUI ();
		
		void RenderPauseMenu ();

		float fixedUpdateIntervalMs { 16 };
		bool quit { false };
		bool paused;
		Window window;
		RenderContext renderContext;
		GUIRenderer guiRenderer;
		std::unordered_map <std::string, GLuint> voxelTypeTextures;
		std::unordered_map <std::string, GLuint> voxelTypeItemTextures;
		GLuint whiteTexture;
		World world;
		std::chrono::steady_clock::time_point lastFrameTime;
		std::chrono::steady_clock::time_point lastFixedUpdateTime;

		friend class VoxelWorld;
	};

	// Implementation
	inline Window & Application::GetWindow () { return window; }
	inline bool Application::CheckPaused () const { return paused; }
}