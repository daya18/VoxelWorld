#include "Application.hpp"

#include "core.hpp"

namespace vw
{
	Application::Application ()
		:
		window { "Voxel World", { 1280, 720 } },
		renderContext ( window ),

		voxelTypeTextures {
			{ "Grass", CreateTextureFromFile ( "image/blocks/Grass.png" ) },
			{ "Dirt", CreateTextureFromFile  ( "image/blocks/Dirt.png" ) },
			{ "Stone", CreateTextureFromFile ( "image/blocks/Stone.png" ) },
			{ "Log", CreateTextureFromFile   ( "image/blocks/Log.png" ) },
			{ "Plank", CreateTextureFromFile ( "image/blocks/Plank.png" ) }
		},

		voxelTypeItemTextures {
			{ "Grass", CreateTextureFromFile ( "image/items/Grass.png" ) },
			{ "Dirt", CreateTextureFromFile  ( "image/items/Dirt.png" ) },
			{ "Stone", CreateTextureFromFile ( "image/items/Stone.png" ) },
			{ "Log", CreateTextureFromFile   ( "image/items/Log.png" ) },
			{ "Plank", CreateTextureFromFile ( "image/items/Plank.png" ) }
		},
		
		whiteTexture ( CreateTextureFromFile ( "image/White.png" ) ),
		world ( *this ),
		guiRenderer ( *this, window )
	{
		SetPaused ( false );

		window.AddKeyDownCallback ( [this] ( int key ) {
			if ( key == GLFW_KEY_ESCAPE )
				SetPaused ( !paused );
		} );

	}

	Application::~Application ()
	{
		{
			auto textures { GetValues <GLuint> ( voxelTypeTextures ) };
			glDeleteTextures ( static_cast <GLsizei> ( textures.size () ), textures.data () );
		}

		{
			auto textures { GetValues <GLuint> ( voxelTypeItemTextures ) };
			glDeleteTextures ( static_cast < GLsizei > ( textures.size () ), textures.data () );
		}

		glDeleteTextures ( 1, &whiteTexture );
	}

	void Application::Run ()
	{
		lastFrameTime = std::chrono::steady_clock::now ();
		lastFixedUpdateTime = std::chrono::steady_clock::now ();

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
			
			{
				auto timeSincelastFixedUpdate { std::chrono::steady_clock::now () - lastFixedUpdateTime };
				auto timeSinceLastFixedUpdateMs { std::chrono::duration_cast < std::chrono::milliseconds > ( timeSincelastFixedUpdate ) };

				if ( timeSinceLastFixedUpdateMs.count () >= fixedUpdateIntervalMs )
				{
					FixedUpdate ();
					lastFixedUpdateTime = std::chrono::steady_clock::now ();
				}
			}

			renderContext.Begin ();
			Render ();
			RenderGUI ();
			renderContext.End ();
		}
	}

	void Application::Update ( float deltaTime )
	{
		window.HandleInput ();
		world.Update ( deltaTime );
	}
	
	void Application::FixedUpdate ()
	{
		world.FixedUpdate ();
	}

	void Application::Render ()
	{
		world.Render ();

		ImGui::ShowMetricsWindow ();
		
		if ( paused )
			RenderPauseMenu ();
	}
	
	void Application::RenderGUI ()
	{
		world.RenderGUI ( guiRenderer );
	}

	void Application::RenderPauseMenu ()
	{
		auto flags { ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove };
		ImGui::Begin ( "Pause Menu", nullptr, flags );

		ImVec2 buttonSize { 150.0f, 50.0f };

		if ( ImGui::Button ( "Resume", buttonSize ) )
			SetPaused ( false );

		if ( ImGui::Button ( "Quit", buttonSize ) )
			quit = true;

		ImGui::SetWindowPos ( ( ImGui::GetIO ().DisplaySize - ImGui::GetWindowSize () ) * 0.5f );

		ImGui::End ();
	}

	void Application::SetPaused ( bool paused )
	{
		this->paused = paused;
		window.SetRawMouseInput ( !paused );
	}

	GLuint Application::GetVoxelTexture ( std::string const & type ) const
	{
		return voxelTypeTextures.at ( type );
	}

	GLuint Application::GetItemTexture ( std::string const & type ) const
	{
		return voxelTypeItemTextures.at ( type );
	}

	GLuint Application::GetWhiteTexture () const
	{
		return whiteTexture;
	}
}