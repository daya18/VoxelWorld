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
		voxelWorld.Fill ( { -15, 0, -15 }, { 15, 0, 15 }, "Grass" );
		//Fill ( { -5, 0, -5 }, { 5, 0, 5 }, "Grass" );

		/*AddVoxel ( { 0, 0, 0 }, "Grass" );
		AddVoxel ( { 1, 0, 0 }, "Grass" );
		AddVoxel ( { 2, 0, 0 }, "Grass" );*/

		//RemoveVoxels ( { { 2, 0, 0 } } );
	}

	void World::Update ( float delta )
	{
		voxelWorld.Update ( delta );
		player.Update ( delta );
	}

	void World::Render ()
	{
		voxelWorld.Render ();
	}
}