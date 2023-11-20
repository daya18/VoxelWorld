#include "VoxelWorld.hpp"

#include "Renderer.hpp"
#include "Window.hpp"
#include "Application.hpp"
#include "Core.hpp"

namespace vw
{
	VoxelWorld::VoxelWorld ( Application & application )
	: 
		application ( &application ),
		window ( & application.window ),
		camera ( *window ),
		renderer ( application, *this ),
		raycaster ( application, *this )
	{
		renderer.SetCamera ( camera );
	}

	void VoxelWorld::AddVoxel ( glm::ivec3 const & index, std::string const & type )
	{
		auto position { IndexToPosition ( index ) };
		voxels.insert ( { position, { *this, { position, type } } } );
		
		UpdateVoxelNeighbours ();
		renderer.Update ();
	}
	
	void VoxelWorld::Fill ( glm::ivec3 const & fromIndex, glm::ivec3 const & toIndex, std::string const & voxelType )
	{
		auto min { glm::min ( fromIndex, toIndex ) };
		auto max { glm::max ( fromIndex, toIndex ) };

		for ( int x = min.x; x <= max.x; ++x )
		{
			for ( int y = min.y; y <= max.y; ++y )
			{
				for ( int z = min.z; z <= max.z; ++z )
				{
					auto position { IndexToPosition ( { x, y, z } ) };
					voxels.insert ( { position, { *this, { position, voxelType } } } );
				}
			}
		}

		UpdateVoxelNeighbours ();
		renderer.Update ();
	}

	void VoxelWorld::RemoveVoxels ( std::vector <glm::ivec3> const & indices )
	{
		for ( auto const & index : indices )
			voxels.erase ( IndexToPosition ( index ) );
		
		UpdateVoxelNeighbours ();
		renderer.Update ();
	}

	void VoxelWorld::Update ( float deltaTime )
	{
		camera.Update ( deltaTime );
		raycaster.Update ();
	}

	void VoxelWorld::Render ()
	{
		renderer.Render ();
	}

	void VoxelWorld::UpdateVoxelNeighbours ()
	{
		for ( auto & [ position, voxel ] : voxels )
			voxel.ClearNeighbours ();

		for ( auto & [ lvoxelPosition, lvoxel] : voxels )
		{
			for ( auto & [rvoxelPosition, rvoxel] : voxels )
			{
				for ( auto const & side : sides )
				{
					if ( lvoxelPosition + GetDirectionVector ( side ) * voxelScale * 2.0f == rvoxelPosition )
					{
						lvoxel.SetNeighbour ( side, rvoxel );
						rvoxel.SetNeighbour ( GetOppositeSide ( side ), lvoxel );
					}
				}
			}
		}
	}

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

	glm::vec3 VoxelWorld::IndexToPosition ( glm::ivec3 const & index )
	{
		return glm::vec3 { index } * voxelScale * 2.0f;
	}

}