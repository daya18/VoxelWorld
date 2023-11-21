#include "VoxelWorldRaycaster.hpp"

#include "VoxelWorld.hpp"
#include "Application.hpp"
#include "core.hpp"

namespace vw
{
	float const VoxelWorldRaycaster::rayLength { 10.0f };

	VoxelWorldRaycaster::VoxelWorldRaycaster ( Application const & application, VoxelWorld & world )
		: application ( & application ), world ( & world )
	{
	}

	void VoxelWorldRaycaster::Update ()
	{
		auto voxelsWithinReach { GetVoxelsWithinReach () };

		bool intersects { false };
		float minIntersectionDistance { std::numeric_limits <float>::max () };
		Voxel * intersectingVoxel { nullptr };
		Sides minIntersectionSide { Sides::left };

		for ( auto & voxelWithinReach : voxelsWithinReach )
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

		targetVoxel = intersectingVoxel;
		targetVoxelSide = minIntersectionSide;
	}

	std::vector <Voxel *> VoxelWorldRaycaster::GetVoxelsWithinReach () const
	{
		std::vector <Voxel *> voxels;
		voxels.reserve ( world->voxels.size () );

		for ( auto & [voxelPosition, voxel] : world->voxels )
		{
			auto cameraVoxelDistance { glm::distance ( world->camera.GetPosition (), voxel.GetPosition () ) };

			if ( cameraVoxelDistance < rayLength )
				voxels.push_back ( & voxel );
		}

		voxels.shrink_to_fit ();
		return voxels;
	}
}