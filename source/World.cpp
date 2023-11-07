#include "World.hpp"

#include "Renderer.hpp"
#include "Window.hpp"

namespace vw
{
	World::World ( Window & window, Renderer & renderer )
	: 
		window ( & window ),
		camera ( window )
	{
		renderer.SetCamera ( camera );
	}

	void World::Update ( float deltaTime )
	{
		camera.Update ( deltaTime );
	}
}