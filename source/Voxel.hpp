#pragma once

#include "Core.hpp"
#include "VoxelModel.hpp"

namespace vw
{
	class VoxelWorld;

	class Voxel
	{
	public:
		struct Definition
		{
			glm::vec3 position;
			std::string type;
		};

		Voxel ( VoxelWorld &, Definition const & );
	
		glm::vec3 const & GetPosition () const;
		std::string const & GetType () const;
		
		std::unordered_map < Sides, Voxel *> const & GetNeighbours () const;
		
		bool CheckNeighbour ( Sides ) const;
		glm::mat4 const & GetTransformMatrix () const;
		bool CheckFaceIntersection ( Sides side, float & distance ) const;

	private:

		VoxelWorld * world;
		glm::vec3 position;
		std::string type;
		glm::mat4 transform;
		std::unordered_map < Sides, Voxel *> neighbours;
		VoxelModel worldSpaceModel;

		friend class VoxelWorld;
	};

	// Implementation
	inline glm::vec3 const & Voxel::GetPosition () const { return position; }
	inline std::string const & Voxel::GetType () const { return type; }
	inline std::unordered_map < Sides, Voxel *> const & Voxel::GetNeighbours () const { return neighbours; }
	inline glm::mat4 const & Voxel::GetTransformMatrix () const { return transform; }
}