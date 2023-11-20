#include "TestVoxelWorld.hpp"

#include "Application.hpp"
#include "Window.hpp"

namespace vw
{
	TestVoxelWorld::TestVoxelWorld ( Application & application )
		: VoxelWorld ( application )
	{
		//Fill ( { 1, -1, -1 }, { -1, 1, 1 }, "Grass" );
		Fill ( { -10, -3, -10 }, { 10, 0, 10 }, "Grass" );

		//AddVoxel ( { 0, 0, 0 }, "Grass" );

		//RemoveVoxels ( { { 0, 0, 0 } } );

		auto & window { application.GetWindow () };
		
		window.AddSignalListener ( "KeyPress", [this, &window] () { 
			if ( window.GetLastKey () == GLFW_KEY_B )
				RemoveVoxels ( { GetVoxels ().back ()->GetPosition () } );
		} );
	}
}