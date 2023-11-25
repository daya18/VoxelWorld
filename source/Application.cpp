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
			{ "Stone", CreateTextureFromFile ( "image/Stone.png" ) },
			{ "Log", CreateTextureFromFile ( "image/Log.png" ) },
			{ "Plank", CreateTextureFromFile ( "image/Plank.png" ) }
		},

		world ( *this )
	{
		SetPaused ( false );

		window.AddKeyDownCallback ( [this] ( int key ) {
			if ( key == GLFW_KEY_ESCAPE )
				SetPaused ( !paused );
		} );

	}

	Application::~Application ()
	{
		auto textures { GetValues <GLuint> ( voxelTypeTextures ) };
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
		
		if ( paused ) 
			return;

		world.Update ( deltaTime );
	}

	void Application::Render ()
	{
		renderContext.Begin ();
		
		world.Render ();
		
		ImGui::ShowMetricsWindow ();

		if ( paused )
		{
			ImGui::Begin ("Pause Menu", nullptr, ImGuiWindowFlags_AlwaysAutoResize );
			
			ImVec2 buttonSize { 150.0f, 50.0f };

			if ( ImGui::Button ( "Resume", buttonSize ) )
				SetPaused ( false );

			if ( ImGui::Button ( "Quit", buttonSize ) )
				quit = true;

			ImGui::End ();
		}

		renderContext.End ();
	}

	void Application::SetPaused ( bool paused )
	{
		this->paused = paused;
		window.SetRawMouseInput ( !paused );
	}
}