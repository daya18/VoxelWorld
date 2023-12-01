#pragma once

#include "Voxel.hpp"
#include "VoxelWorldRaycaster.hpp"
#include "opengl/VectorBuffer.hpp"
#include "opengl/MapBuffer.hpp"
#include "opengl/Shader.hpp"
#include "Mesh.hpp"

namespace vw
{
	class Application;
	class Window;
	class Camera;

	class VoxelWorld
	{
	public:
		VoxelWorld ( Application & );

		void AddVoxel ( glm::vec3 const & position, std::string const & type );
		void Fill ( glm::vec3 const & from, glm::vec3 const & to, std::string const & voxelType );
		void RemoveVoxel ( Voxel * voxel );

		Voxel * FindVoxel ( glm::vec3 const & position );

		void SetCamera ( Camera & );

		void Update ( float deltaTime );
		void Render ();

		std::vector <Voxel *> GetVoxels ();
		std::vector <Voxel *> GetVoxels ( glm::vec3 const & centre, float radius );

		float GetVoxelScale () const;
		Application const & GetApplication () const;
		VoxelWorldRaycaster & GetRaycaster ();
		unsigned int GetVoxelCount () const;

	private:
		struct VoxelData
		{
			glm::mat4 transform;
		};

		void RenderVoxelOutline ( glm::mat4 const & transform );
		
		float voxelScale = 1.0f;

		Application * application;
		Window * window;
		Camera * camera;
		std::unordered_map <glm::vec3, Voxel> voxels;

		VoxelWorldRaycaster raycaster;

		Mesh voxelMesh;
		Mesh voxelOutlineMesh;
		Shader shaderProgram;
		Shader simpleShaderProgram;
		std::unordered_map <std::string, MapBuffer <Voxel *, VoxelData>> typeVoxelBuffers;

		friend class Voxel;
		friend class VoxelWorldRaycaster;
	};
	
	// Implementation
	inline float VoxelWorld::GetVoxelScale () const { return voxelScale; }
	inline Application const & VoxelWorld::GetApplication () const { return *application; }
	inline VoxelWorldRaycaster & VoxelWorld::GetRaycaster () { return raycaster; }
	inline unsigned int VoxelWorld::GetVoxelCount () const { return static_cast < unsigned int > ( voxels.size () ); }

}