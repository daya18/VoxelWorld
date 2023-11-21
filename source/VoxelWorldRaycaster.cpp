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

		bool intersects { false };
		float minIntersectionDistance { std::numeric_limits <float>::max () };
		Voxel const * intersectingVoxel;
		Sides minIntersectionSide;

		for ( auto const & voxelWithinReach : voxelsWithinReach )
		{
			float intersectionDistance;
			Sides intersectionSide;

			if ( voxelWithinReach->CheckCameraRayIntersection ( intersectionDistance, intersectionSide ) )
			{
				if ( intersectionDistance < minIntersectionDistance )
				{
					intersects = true;
					minIntersectionDistance = intersectionDistance;
					intersectingVoxel = voxelWithinReach;
					minIntersectionSide = intersectionSide;
				}
			}
		}

		if ( intersects )
		{
			std::cout << glm::to_string ( intersectingVoxel->GetPosition () ) << ' ' << sideNames.at ( minIntersectionSide ) << std::endl;
		}
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