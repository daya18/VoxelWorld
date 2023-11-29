#pragma once

#include "Core.hpp"
#include "VoxelModel.hpp"
#include <ei/3dtypes.hpp>

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
		bool CheckNeighbour ( Sides ) const;
		glm::mat4 const & GetTransformMatrix () const;
		bool CheckCameraRayIntersection ( float & distance, Sides & hitSide ) const;
		std::vector <Voxel *> GetNeighbours () const;
		bool CheckVisible () const;

	private:
		void ClearNeighbours ();
		void SetNeighbour ( Sides, Voxel & );

		VoxelWorld * world;
		glm::vec3 position;
		std::string type;
		glm::mat4 transform;
		std::array <Voxel *, 6> neighbours;
		ei::Box eiBox;

		friend class VoxelWorld;
	};

	// Implementation
	inline glm::vec3 const & Voxel::GetPosition () const { return position; }
	inline std::string const & Voxel::GetType () const { return type; }
	inline glm::mat4 const & Voxel::GetTransformMatrix () const { return transform; }
}