#pragma once

#include "Window.hpp"
#include "Renderer.hpp"
#include "World.hpp"

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

	private:
		bool quit { false };
		Window window;
		Renderer renderer;
		glm::mat4 projectionMatrix;
		World world;
	};
}