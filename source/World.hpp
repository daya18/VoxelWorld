#pragma once

#include "VoxelWorld.hpp"
#include "Player.hpp"

namespace vw
{
	class World
	{
	public:
		World ( Application & application );

		void Update ( float );
		void Render ();

		Application & GetApplication ();
		VoxelWorld & GetVoxelWorld ();

	private:
		Application * application;
		VoxelWorld voxelWorld;
		Player player;
	};



	// Implementation
	inline Application & World::GetApplication () { return *application; }
	inline VoxelWorld & World::GetVoxelWorld () { return voxelWorld; }
}