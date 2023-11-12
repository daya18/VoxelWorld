#include "TestVoxelWorld.hpp"

#include "Application.hpp"
#include "Window.hpp"

namespace vw
{
	TestVoxelWorld::TestVoxelWorld ( Application & application )
		: VoxelWorld ( application )
	{
		std::vector <Voxel::Definition> voxelDefinitions;

		for ( int x = -10; x < 10; ++x )
		{
			for ( int z = -10; z < 10; ++z )
			{
				voxelDefinitions.push_back ( { { x, -5, z }, "Grass" } );
			}
		}
		
		AddVoxels ( voxelDefinitions );

		auto & window { application.GetWindow () };
		
		window.AddSignalListener ( "KeyPress", [this, &window] () { 
			if ( window.GetLastKey () == GLFW_KEY_B )
				RemoveVoxels ( { GetVoxels ().back ()->GetPosition () } );
		} );
	}
}