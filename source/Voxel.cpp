#include "Voxel.hpp"

#include "VoxelWorld.hpp"

namespace vw
{
	std::unordered_map <Voxel::Sides, glm::ivec3> Voxel::directionVectors
	{
		{ Sides::up,		{  0,  1,  0 } },
		{ Sides::down,		{  0, -1,  0 } },
		{ Sides::left,		{ -1,  0,  0 } },
		{ Sides::right,		{  1,  0,  0 } },
		{ Sides::forward,	{  0,  0,  1 } },
		{ Sides::back,		{  0,  0, -1 } },
	};
	
	std::unordered_map <Voxel::Sides, Voxel::Sides> Voxel::oppositeSides
	{
		{ Sides::up,		Sides::down },
		{ Sides::down,		Sides::up },
		{ Sides::left,		Sides::right },
		{ Sides::right,		Sides::left },
		{ Sides::forward,	Sides::back },
		{ Sides::back,		Sides::forward },
	};

	std::array <Voxel::Sides, 6> Voxel::sides { Sides::up, Sides::down, Sides::left, Sides::right, Sides::forward, Sides::back };

	Voxel::Voxel ( VoxelWorld & world, Definition const & definition )
		: world ( & world ), position ( definition.position ), type ( definition.type )
	{

	}

	bool Voxel::CheckNeighbour ( Sides side ) const
	{
		return world->voxels.find ( position + directionVectors [ side ] ) != world->voxels.end ();
	}

}
