#include "VoxelWorld.hpp"

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
		raycaster ( application, *this ),
		simpleRenderer ( application, *this )
	{
		simpleRenderer.SetCamera ( camera );
	}

	void VoxelWorld::AddVoxel ( glm::vec3 const & position, std::string const & type )
	{
		auto & voxel { voxels.insert ( { position, { *this, { position, type } } } ).first->second };
		
		UpdateVoxelNeighbours ( voxel );

		for ( auto neighbour : voxel.GetNeighbours () )
			UpdateVoxelNeighbours ( *neighbour );

		simpleRenderer.AddVoxels ( { &voxel } );
	}	
	
	void VoxelWorld::Fill ( glm::vec3 const & fromPosition, glm::vec3 const & toPosition, std::string const & voxelType )
	{
		auto min { glm::min ( fromPosition, toPosition ) };
		auto max { glm::max ( fromPosition, toPosition ) };

		// TO DO: Optimize using std::vector::reserve

		std::vector <Voxel *> newVoxels;
		
		for ( float x = min.x; x <= max.x; ++x )
		{
			for ( float y = min.y; y <= max.y; ++y )
			{
				for ( float z = min.z; z <= max.z; ++z )
				{
					glm::vec3 position { x, y, z };
					newVoxels.push_back ( &voxels.insert ( { position, { *this, { position, voxelType } } } ).first->second );
				}
			}
		}

		UpdateAllVoxelNeighbours ();

		simpleRenderer.AddVoxels ( newVoxels );
	}

	void VoxelWorld::RemoveVoxels ( std::vector <Voxel *> const & voxels )
	{
		std::vector <Voxel *> neighbours;
		neighbours.reserve ( voxels.size () * 6 );

		for ( auto const & voxel : voxels )
		{
			auto voxelNeighbours { voxel->GetNeighbours () };
			neighbours.insert ( neighbours.end (), voxelNeighbours.begin (), voxelNeighbours.end () );
			
			this->voxels.erase ( voxel->GetPosition () );
			
			if ( voxel == raycaster.GetTargetVoxel () )
				raycaster.Clear ();
		}
		
		for ( auto & voxel : neighbours )
				UpdateVoxelNeighbours ( *voxel );
		
		simpleRenderer.RemoveVoxels ( voxels );
	}

	void VoxelWorld::Update ( float deltaTime )
	{
		camera.Update ( deltaTime );
		raycaster.Update ();
	}

	void VoxelWorld::Render ()
	{
		simpleRenderer.Render ();
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

	std::vector <Voxel *> VoxelWorld::GetVoxels ( glm::vec3 const & centre, float radius )
	{
		auto diameter { radius * 2.0f };

		std::vector < Voxel *> voxels;
		voxels.reserve ( this->voxels.size () );

		glm::vec3 begin { centre - radius };
		glm::vec3 end { centre + radius };

		
		for ( float x { begin.x }; x <= end.x; x += 1.0f )
		{
			for ( float y { begin.y }; y <= end.y; y += 1.0f )
			{
				for ( float z { begin.z }; z <= end.z; z += 1.0f )
				{
					auto voxelIt { this->voxels.find ( glm::vec3 { x, y, z } ) };
					
					if ( voxelIt != this->voxels.end () )
						voxels.push_back ( &voxelIt->second );
				}
			}
		}			
		
		return voxels;
	}
}