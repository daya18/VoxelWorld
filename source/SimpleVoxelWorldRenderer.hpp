#pragma once

#include "Voxel.hpp"
#include "opengl/VectorBuffer.hpp"
#include "Mesh.hpp"

namespace vw
{
	class Application;
	class Camera;

	class SimpleVoxelWorldRenderer
	{
	public:
		SimpleVoxelWorldRenderer ( Application &, VoxelWorld & );
		SimpleVoxelWorldRenderer ( SimpleVoxelWorldRenderer const & ) = delete;
		SimpleVoxelWorldRenderer ( SimpleVoxelWorldRenderer && );
		~SimpleVoxelWorldRenderer ();

		SimpleVoxelWorldRenderer & operator = ( SimpleVoxelWorldRenderer const & ) = delete;
		SimpleVoxelWorldRenderer & operator = ( SimpleVoxelWorldRenderer && );

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

		Application const * application;
		VoxelWorld * world;

		bool destroyResources;
		Mesh voxelMesh;
		Mesh voxelOutlineMesh;
		
		GLuint shaderProgram;
		GLuint simpleShaderProgram;

		VoxelWorld * voxelWorld;
		
		// Uniform locations
		GLuint modelMatricesUniformLocation;
		GLuint viewMatrixUniformLocation;
		GLuint projectionMatrixUniformLocation;
		GLuint colorUniformLocation;
		
		VectorBuffer <VoxelData> voxelBuffer;
		GLuint voxelCount { 0 };
	};
}