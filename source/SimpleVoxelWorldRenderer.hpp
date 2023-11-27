#pragma once

#include "Voxel.hpp"
#include "opengl/VectorBuffer.hpp"
#include "opengl/Shader.hpp"
#include "Mesh.hpp"

namespace vw
{
	class Application;
	class Camera;

	class SimpleVoxelWorldRenderer
	{
	public:
		SimpleVoxelWorldRenderer ( Application &, VoxelWorld & );

		void SetCamera ( Camera & );

		void SetVoxelScale ( float );
		float GetVoxelScale () const;

		void AddVoxels ( std::vector <Voxel *> const & );
		void RemoveVoxels ( std::vector <Voxel *> const & );

		void Render ();

	private:
		void RenderOutline ( glm::mat4 const & transform );
		void Update ();

		struct VoxelData
		{
			glm::mat4 transform;
		};
		
		std::vector <glm::mat4> GetVoxelTransforms ( std::vector <Voxel *> const & );

		void SetView ( glm::mat4 const & );
		void SetProjection ( glm::mat4 const & );

		// Initialization helpers
		void CreateShaderProgram ();

		bool destroyResources;
		Application const * application;
		VoxelWorld * world;

		Mesh voxelMesh;
		Mesh voxelOutlineMesh;
		
		Shader shaderProgram;
		Shader simpleShaderProgram;
		
		VectorBuffer <VoxelData> voxelBuffer;
		GLuint voxelCount { 0 };
	};
}