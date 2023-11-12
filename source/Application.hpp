#pragma once

#include "Window.hpp"
#include "Renderer.hpp"
#include "VoxelWorld.hpp"

namespace vw
{
	class Application
	{
	public:
		Application ();
		Application ( Application const & ) = delete;
		Application ( Application && ) = delete;

		Application & operator = ( Application const & ) = delete;
		Application & operator = ( Application && ) = delete;

		void Run ();
		Window & GetWindow ();

	private:
		bool quit { false };
		Window window;
		Renderer renderer;
		std::unordered_map <std::string, GLuint> voxelTypeTextures;
		std::unique_ptr <VoxelWorld> world;

		friend class VoxelWorld;
		friend class VoxelWorldRenderer;
	};

	// Implementation
	inline Window & Application::GetWindow () { return window; }

}