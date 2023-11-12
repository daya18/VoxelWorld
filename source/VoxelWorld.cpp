#include "VoxelWorld.hpp"

#include "Renderer.hpp"
#include "Window.hpp"
#include "Application.hpp"

namespace vw
{
	VoxelWorld::VoxelWorld ( Application & application )
	: 
		window ( & application.window ),
		camera ( *window ),
		renderer ( application, *this )
	{
		renderer.SetCamera ( camera );
	}

	void VoxelWorld::AddVoxels ( std::vector <Voxel::Definition> const & definitions )
	{
		for ( auto const & definition : definitions )
			voxels.insert ( { definition.position, { *this, definition } } );

		renderer.Update ();
	}

	void VoxelWorld::RemoveVoxels ( std::vector <glm::ivec3> const & positions )
	{
		for ( auto const & position : positions )
			voxels.erase ( position );
		
		renderer.Update ();
	}

	void VoxelWorld::Update ( float deltaTime )
	{
		camera.Update ( deltaTime );
	}

	void VoxelWorld::Render ()
	{
		renderer.Render ();
	}

	/*void VoxelWorld::UpdateVoxelNeighbours ()
	{
		for ( auto & [ position, voxel ] : voxels )
			voxel.neighbours.clear ();

		for ( auto & [ lvoxelPosition, lvoxel] : voxels )
		{
			for ( auto & [rvoxelPosition, rvoxel] : voxels )
			{
				for ( auto const & side : Voxel::sides )
				{
					if ( lvoxelPosition + Voxel::directionVectors [ side ] == rvoxelPosition )
					{
						lvoxel.neighbours [side] = &rvoxel;
						rvoxel.neighbours [ Voxel::oppositeSides [ side ] ] = &lvoxel;
					}
				}
			}
		}
	}*/

	Voxel * VoxelWorld::FindVoxel ( glm::ivec3 const & position )
	{
		auto voxelIt { voxels.find ( position ) };
		if ( voxelIt == voxels.end () ) return nullptr;
		return &voxelIt->second;
	}

	std::vector <Voxel *> VoxelWorld::GetVoxels ()
	{
		std::vector <Voxel *> outVoxels;
		outVoxels.reserve ( voxels.size () );

		for ( auto & [ pos, vox ] : voxels )
			outVoxels.push_back ( &vox );
		
		return outVoxels;
	}
}