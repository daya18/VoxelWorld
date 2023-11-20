#pragma once

#include "Voxel.hpp"
#include "Camera.hpp"
#include "VoxelWorldRenderer.hpp"
#include "VoxelWorldRaycaster.hpp"

namespace vw
{
	class Application;
	class Window;

	class VoxelWorld
	{
	public:
		VoxelWorld ( Application & );

		void AddVoxel ( glm::ivec3 const & position, std::string const & type );
		void Fill ( glm::ivec3 const & from, glm::ivec3 const & to, std::string const & voxelType );
		void RemoveVoxels ( std::vector <glm::ivec3> const & positions );

		Voxel * FindVoxel ( glm::ivec3 const & position );

		void Update ( float deltaTime );
		void Render ();

		std::vector <Voxel *> GetVoxels ();

		float GetVoxelScale () const;
		Application const & GetApplication () const;

	private:
		glm::vec3 IndexToPosition ( glm::ivec3 const & );
		void UpdateVoxelNeighbours ();

		float voxelScale = 1.0f;

		Application * application;
		Window * window;
		Camera camera;
		std::unordered_map <glm::vec3, Voxel> voxels;

		VoxelWorldRenderer renderer;
		VoxelWorldRaycaster raycaster;

		friend class Voxel;
		friend class VoxelWorldRenderer;
		friend class VoxelWorldRaycaster;
	};
	
	// Implementation
	inline float VoxelWorld::GetVoxelScale () const { return voxelScale; }
	inline Application const & VoxelWorld::GetApplication () const { return *application; }
}