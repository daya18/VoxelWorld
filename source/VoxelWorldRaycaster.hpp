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
		VoxelWorldRaycaster ( Application const &, VoxelWorld const & );

		void Update ();

	private:
		//static std::string GetVoxelFaceString ( Voxel::sides side );
		
		std::vector <Voxel const *> GetVoxelsWithinReach () const;

		static const float rayLength;

		Application const * application;
		VoxelWorld const * world;
	};
}