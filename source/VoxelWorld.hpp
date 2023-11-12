#pragma once

#include "Voxel.hpp"
#include "Camera.hpp"
#include "VoxelWorldRenderer.hpp"

namespace vw
{
	class Application;
	class Window;

	class VoxelWorld
	{
	public:
		VoxelWorld ( Application & );

		void AddVoxels ( std::vector <Voxel::Definition> const & definitions );
		void RemoveVoxels ( std::vector <glm::ivec3> const & positions );

		Voxel * FindVoxel ( glm::ivec3 const & position );

		void Update ( float deltaTime );
		void Render ();

		std::vector <Voxel *> GetVoxels ();

	private:
		void UpdateVoxelNeighbours ();

		Window * window;
		Camera camera;
		std::unordered_map <glm::ivec3, Voxel> voxels;

		VoxelWorldRenderer renderer;

		friend class Voxel;
		friend class VoxelWorldRenderer;
	};
}