#include "VoxelWorldRaycaster.hpp"

#include "VoxelWorld.hpp"
#include "Application.hpp"
#include "Utility.hpp"

namespace vw
{
	float const VoxelWorldRaycaster::rayLength { 10.0f };

	VoxelWorldRaycaster::VoxelWorldRaycaster ( Application const & application, VoxelWorld const & world )
		: application ( & application ), world ( & world )
	{
	}

	void VoxelWorldRaycaster::Update ()
	{
		auto voxelsWithinReach { GetVoxelsWithinReach () };

		for ( auto const & voxelWithinReach : voxelsWithinReach )
		{
			bool intersects { false };
			float minIntersectionDistance { std::numeric_limits <float>::max () };
			Sides minIntersectionSide;

			for ( auto const & side : sides )
			{
				float intersectionDistance;

				if ( voxelWithinReach->CheckFaceIntersection ( side, intersectionDistance ) )
				{
					if ( intersectionDistance < minIntersectionDistance )
					{
						intersects = true;
						minIntersectionDistance = intersectionDistance;
						minIntersectionSide = side;
					}
				}
			}
/*
			if ( intersects )
				std::cout << sideNames.at ( minIntersectionSide ) << std::endl;
	*/	}
	}

	std::vector <Voxel const *> VoxelWorldRaycaster::GetVoxelsWithinReach () const
	{
		std::vector <Voxel const *> voxels;
		voxels.reserve ( world->voxels.size () );

		for ( auto const & [voxelPosition, voxel] : world->voxels )
		{
			auto cameraVoxelDistance { glm::distance ( world->camera.GetPosition (), voxel.GetPosition () ) };

			if ( cameraVoxelDistance < rayLength )
				voxels.push_back ( & voxel );
		}

		voxels.shrink_to_fit ();
		return voxels;
	}

	
}