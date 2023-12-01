#pragma once

#include "Core.hpp"

namespace vw
{
	class Voxel;
	class VoxelWorld;
	class Application;

	class VoxelWorldRaycaster
	{
	public:
		VoxelWorldRaycaster ( Application const &, VoxelWorld & );

		void Update ();
		Voxel * GetTargetVoxel () const;
		Sides GetTargetVoxelSide () const;
		void Clear ();

	private:
		static const float rayLength;

		Application const * application;
		VoxelWorld * world;
		Voxel * targetVoxel { nullptr };
		Sides targetVoxelSide;
	};



	// Implementation
	inline Voxel * VoxelWorldRaycaster::GetTargetVoxel () const { return targetVoxel; }
	inline Sides VoxelWorldRaycaster::GetTargetVoxelSide () const { return targetVoxelSide; }
}