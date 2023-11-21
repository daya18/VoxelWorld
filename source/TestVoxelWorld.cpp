#include "TestVoxelWorld.hpp"

#include "Application.hpp"
#include "Window.hpp"

namespace vw
{
	TestVoxelWorld::TestVoxelWorld ( Application & application )
		: VoxelWorld ( application )
	{
		Fill ( { -15, 0, -15 }, { 15, 0, 15 }, "Grass" );
		//Fill ( { -5, 0, -5 }, { 5, 0, 5 }, "Grass" );

		/*AddVoxel ( { 0, 0, 0 }, "Grass" );
		AddVoxel ( { 1, 0, 0 }, "Grass" );
		AddVoxel ( { 2, 0, 0 }, "Grass" );*/

		//RemoveVoxels ( { { 2, 0, 0 } } );

		auto & window { application.GetWindow () };
		
		window.AddSignalListener ( "KeyPress", [this, &window] () { 
			if ( window.GetLastKey () == GLFW_KEY_B )
				RemoveVoxels ( { GetVoxels ().back () } );
		} );
	}
}