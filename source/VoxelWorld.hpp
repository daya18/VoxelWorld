#pragma once

#include "Voxel.hpp"
#include "Camera.hpp"
#include "VoxelWorldRaycaster.hpp"
#include "SimpleVoxelWorldRenderer.hpp"

namespace vw
{
	class Application;
	class Window;

	class VoxelWorld
	{
	public:
		VoxelWorld ( Application & );

		void AddVoxel ( glm::vec3 const & position, std::string const & type );
		void Fill ( glm::vec3 const & from, glm::vec3 const & to, std::string const & voxelType );
		void RemoveVoxels ( std::vector <Voxel *> const & voxels );

		Voxel * FindVoxel ( glm::vec3 const & position );

		void Update ( float deltaTime );
		void Render ();

		std::vector <Voxel *> GetVoxels ();
		std::vector <Voxel *> GetVoxels ( glm::vec3 const & centre, float radius );

		float GetVoxelScale () const;
		Application const & GetApplication () const;
		VoxelWorldRaycaster & GetRaycaster ();
		unsigned int GetVoxelCount () const;

	private:
		void UpdateAllVoxelNeighbours ();
		void UpdateVoxelNeighbours ( Voxel & );

		float voxelScale = 1.0f;

		Application * application;
		Window * window;
		Camera camera;
		std::unordered_map <glm::vec3, Voxel> voxels;

		SimpleVoxelWorldRenderer simpleRenderer;
		VoxelWorldRaycaster raycaster;

		friend class Voxel;
		friend class SimpleVoxelWorldRenderer;
		friend class VoxelWorldRaycaster;
	};
	
	// Implementation
	inline float VoxelWorld::GetVoxelScale () const { return voxelScale; }
	inline Application const & VoxelWorld::GetApplication () const { return *application; }
	inline VoxelWorldRaycaster & VoxelWorld::GetRaycaster () { return raycaster; }
	inline unsigned int VoxelWorld::GetVoxelCount () const { return static_cast < unsigned int > ( voxels.size () ); }

}