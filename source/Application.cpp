#include "Application.hpp"

#include "core.hpp"

namespace vw
{
	Application::Application ()
	: 
		window { "Voxel World", { 1280, 720 } },
		renderContext ( window ),
		
		voxelTypeTextures {
			{ "Grass", CreateTextureFromFile ( "image/Grass.png" ) },
			{ "Dirt", CreateTextureFromFile ( "image/Dirt.png" ) },
			{ "Stone", CreateTextureFromFile ( "image/Stone.png" ) }
		},

		world ( *this )
	{
		window.EnableRawMouseInput ();
	}

	Application::~Application ()
	{
		std::vector <GLuint> textures;
		textures.reserve ( voxelTypeTextures.size () + 1 );
		
		for ( auto const & voxelTypeTexture : voxelTypeTextures )
			textures.push_back ( voxelTypeTexture.second );

		glDeleteTextures ( textures.size (), textures.data () );
	}

	void Application::Run ()
	{
		lastFrameTime = std::chrono::steady_clock::now ();

		while ( ! quit )
		{
			auto now { std::chrono::steady_clock::now () };
			auto deltaTimeDuration { now - lastFrameTime };
			auto deltaTimeMs { std::chrono::duration_cast < std::chrono::milliseconds > ( deltaTimeDuration ).count () };
			float deltaTime { static_cast < float > ( deltaTimeMs ) / 1000.0f };
			lastFrameTime = now;

			glfwPollEvents ();
			
			if ( window.ShouldClose () )
			{
				quit = true;
				break;
			}

			Update ( deltaTime );
			Render ();
		}
	}

	void Application::Update ( float deltaTime )
	{
		window.HandleInput ();
		world.Update ( deltaTime );
	}

	void Application::Render ()
	{
		renderContext.Begin ();
		world.Render ();
		renderContext.End ();
	}
}