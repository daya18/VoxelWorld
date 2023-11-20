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
			unsigned int voxelCount;
			GLuint transformIndexBuffer;
		};
		
		GLuint GetFaceIndexOffset ( Sides );
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
		
		// Uniform locations
		GLuint modelMatricesUniformLocation;
		GLuint viewMatrixUniformLocation;
		GLuint projectionMatrixUniformLocation;
		GLuint colorUniformLocation;

		GLuint transformBuffer;
		
						// Texture
		std::unordered_map <GLuint, std::unordered_map <Sides, Batch> >batches;
		
	};
}