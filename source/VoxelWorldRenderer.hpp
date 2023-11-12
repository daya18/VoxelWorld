#pragma once

#include "VectorBuffer.hpp"
#include "Voxel.hpp"

namespace vw
{
	class Application;
	class Camera;

	class VoxelWorldRenderer
	{
	public:
		VoxelWorldRenderer ( Application &, VoxelWorld & );
		VoxelWorldRenderer ( VoxelWorldRenderer const & ) = delete;
		VoxelWorldRenderer ( VoxelWorldRenderer && );
		~VoxelWorldRenderer ();

		VoxelWorldRenderer & operator = ( VoxelWorldRenderer const & ) = delete;
		VoxelWorldRenderer & operator = ( VoxelWorldRenderer && );

		void SetCamera ( Camera & );

		void SetVoxelScale ( float );
		float GetVoxelScale () const;

		void Update ();
		void Render ();

	private:
		struct Batch
		{
			VectorBuffer <glm::vec4> transformIndexBuffer;
		};
		
		GLuint GetFaceIndexOffset ( Voxel::Sides );
		void SetView ( glm::mat4 const & );
		void SetProjection ( glm::mat4 const & );

		void CreateGeometryBuffers ();
		GLuint CreateShader ( std::string const & filePath, GLenum type );
		void CreateShaderProgram ();
		void CreateVertexArray ();

		Application const * application;
		VoxelWorld const * world;

		bool destroyResources;
		GLuint vertexBuffer;
		GLuint indexBuffer;
		GLuint shaderProgram;
		GLuint vertexArray;
		
		VoxelWorld * voxelWorld;
		float voxelScale = 1.0f;
		
		// Uniform locations
		GLuint modelMatricesUniformLocation;
		GLuint viewMatrixUniformLocation;
		GLuint projectionMatrixUniformLocation;
		GLuint colorUniformLocation;

		GLuint transformBuffer;
		//std::vector <Voxel *> voxels;
		//VectorBuffer <glm::mat4> transformBuffer;
		
						// Texture
		std::unordered_map <GLuint, std::unordered_map <Voxel::Sides, Batch> >batches;
		
	};

	// Implementation
	inline void VoxelWorldRenderer::SetVoxelScale ( float scale ) { voxelScale = scale; }
	inline float VoxelWorldRenderer::GetVoxelScale () const { return voxelScale; }
}