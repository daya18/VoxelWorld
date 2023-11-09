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

		renderer.GetVoxelRenderer ().AddVoxelType ( "Grass", "image/TestBlockTexture.png" );

		renderer.GetVoxelRenderer ().AddVoxel ( { 0.0f, 0.0f, 0.0f }, "Grass" );
		renderer.GetVoxelRenderer ().AddVoxel ( { 2.0f, 0.0f, 0.0f }, "Grass");
	}

	void World::Update ( float deltaTime )
	{
		camera.Update ( deltaTime );
	}
}