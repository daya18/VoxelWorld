#pragma once

namespace vw
{
	class VoxelWorld;

	class Voxel
	{
	public:
		struct Definition
		{
			glm::ivec3 position;
			std::string type;
		};

		enum class Sides { up, down, left, right, forward, back };
		
		static std::unordered_map <Sides, glm::ivec3> directionVectors;
		static std::unordered_map <Sides, Sides> oppositeSides;
		static std::array <Sides, 6> sides;

		Voxel ( VoxelWorld &, Definition const & );
	
		glm::ivec3 const & GetPosition () const;
		std::string const & GetType () const;
		
		//std::unordered_map < Sides, Voxel *> const & GetNeighbours () const;
		
		bool CheckNeighbour ( Sides ) const;

	private:
		VoxelWorld * world;
		glm::ivec3 position;
		std::string type;
		std::unordered_map < Sides, Voxel *> neighbours;

		friend class VoxelWorld;
	};

	// Implementation
	inline glm::ivec3 const & Voxel::GetPosition () const { return position; }
	inline std::string const & Voxel::GetType () const { return type; }
	//inline std::unordered_map < Voxel::Sides, Voxel *> const & Voxel::GetNeighbours () const { return neighbours; }
}