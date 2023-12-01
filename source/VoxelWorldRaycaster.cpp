#include "VoxelWorldRaycaster.hpp"

#include "VoxelWorld.hpp"
#include "Application.hpp"
#include "core.hpp"

namespace vw
{
	float const VoxelWorldRaycaster::rayLength { 4.0f };

	VoxelWorldRaycaster::VoxelWorldRaycaster ( Application const & application, VoxelWorld & world )
		: application ( & application ), world ( & world )
	{
	}

	void VoxelWorldRaycaster::Update ()
	{
		auto voxelsWithinReach { world->GetVoxels ( glm::round ( world->camera->GetPosition () ), rayLength ) };

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

	void VoxelWorldRaycaster::Clear ()
	{
		targetVoxel = nullptr;
	}
}