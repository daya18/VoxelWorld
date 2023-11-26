#include "World.hpp"

#include "Application.hpp"

namespace vw
{
	World::World ( Application & application )
	: 
		application ( &application ),
		voxelWorld ( application ),
		player ( *this )
	{
		//voxelWorld.Fill ( { -20, -20, -20 }, { 20, 0, 20 }, "Grass" );
		voxelWorld.Fill ( { -5, -5, -5 }, { 5, 0, 5 }, "Grass" );

		//voxelWorld.AddVoxel ( { 0, 0, 0 }, "Grass" );
		/*voxelWorld.AddVoxel ( { 1, 0, 0 }, "Log" );
		voxelWorld.AddVoxel ( { 0, 1, 0 }, "Log" );
		voxelWorld.AddVoxel ( { 1, 1, 0 }, "Log" );*/

		//RemoveVoxels ( { { 2, 0, 0 } } );
		
		std::cout << "Voxel count: " << voxelWorld.GetVoxelCount () << std::endl;
	}

	void World::Update ( float delta )
	{
		voxelWorld.Update ( delta );
		player.Update ( delta );
	}

	void World::Render ()
	{
		voxelWorld.Render ();
		RenderGUI ();
	}

	void World::RenderGUI ()
	{
		ImGui::SetNextWindowPos ( { 0, 0 } );
		ImGui::SetNextWindowSize ( ImGui::GetIO ().DisplaySize );
		ImGui::Begin ( "GUI Panel", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration );
		DrawCrosshair ();
		ImGui::End ();
	}

	void World::DrawCrosshair ()
	{
		ImVec2 size { 30.0f, 30.0f };
		ImVec2 center { ImGui::GetIO ().DisplaySize * 0.5f };
		ImU32 color { ImGui::GetColorU32 ( { 255, 255, 255, 255 } ) };
		float thickness { 3.0f };

		auto drawList { ImGui::GetWindowDrawList () };
		
		// Horizontal line
		drawList->AddLine ( 
			{ center.x - size.x * 0.5f, center.y },
			{ center.x + size.x * 0.5f, center.y },
			color,
			thickness
		);

		// vertical line
		drawList->AddLine (
			{ center.x, center.y - size.y * 0.5f },
			{ center.x, center.y + size.y * 0.5f },
			color,
			thickness
		);
	}
}