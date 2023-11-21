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

	void VoxelWorld::AddVoxel ( glm::vec3 const & position, std::string const & type )
	{
		voxels.insert ( { position, { *this, { position, type } } } );
		
		UpdateAllVoxelNeighbours ();
		renderer.Update ();
	}
	
	void VoxelWorld::Fill ( glm::vec3 const & fromPosition, glm::vec3 const & toPosition, std::string const & voxelType )
	{
		auto min { glm::min ( fromPosition, toPosition ) };
		auto max { glm::max ( fromPosition, toPosition ) };

		for ( int x = min.x; x <= max.x; ++x )
		{
			for ( int y = min.y; y <= max.y; ++y )
			{
				for ( int z = min.z; z <= max.z; ++z )
				{
					glm::vec3 position { x, y, z };
					voxels.insert ( { position, { *this, { position, voxelType } } } );
				}
			}
		}

		UpdateAllVoxelNeighbours ();
		renderer.Update ();
	}

	void VoxelWorld::RemoveVoxels ( std::vector <Voxel *> const & voxels )
	{
		std::vector <Voxel *> affectedVoxels;
		affectedVoxels.reserve ( voxels.size () * 6 );

		for ( auto const & voxel : voxels )
		{
			affectedVoxels.insert ( affectedVoxels.end (), voxel->neighbours.begin (), voxel->neighbours.end () );
			this->voxels.erase ( voxel->GetPosition () );
		}
		
		for ( auto & voxel : affectedVoxels )
			if ( voxel != nullptr )
				UpdateVoxelNeighbours ( *voxel );

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
	
	void VoxelWorld::UpdateAllVoxelNeighbours ()
	{
		for ( auto & [position, voxel] : voxels )
			UpdateVoxelNeighbours ( voxel );
	}

	void VoxelWorld::UpdateVoxelNeighbours ( Voxel & voxel )
	{
		voxel.neighbours = {
			FindVoxel ( voxel.GetPosition () + GetDirectionVector ( Sides::up ) ),
			FindVoxel ( voxel.GetPosition () + GetDirectionVector ( Sides::down ) ),
			FindVoxel ( voxel.GetPosition () + GetDirectionVector ( Sides::left ) ),
			FindVoxel ( voxel.GetPosition () + GetDirectionVector ( Sides::right ) ),
			FindVoxel ( voxel.GetPosition () + GetDirectionVector ( Sides::forward ) ),
			FindVoxel ( voxel.GetPosition () + GetDirectionVector ( Sides::back ) ),
		};
	}

	Voxel * VoxelWorld::FindVoxel ( glm::vec3 const & position )
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