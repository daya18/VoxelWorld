#pragma once

#include "Voxel.hpp"
#include "buffer/MapBuffer.hpp"

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

		void AddVoxels ( std::vector <Voxel *> const & );
		void UpdateVoxels ( std::vector <Voxel *> const & );
		void RemoveVoxels ( std::vector <Voxel *> const & );

		void Update ();
		void Render ();

	private:
		void UpdateBatches ();
		std::vector <glm::mat4> GetVoxelTransforms ( std::vector <Voxel *> const & );
		
		struct VoxelData
		{
			glm::mat4 transform;
		};

		struct Batch
		{
			unsigned int voxelCount;
			MapBuffer <Voxel *, glm::vec4> transformIndexBuffer;
		};
		
		GLuint GetFaceIndexOffset ( Sides );
		void SetView ( glm::mat4 const & );
		void SetProjection ( glm::mat4 const & );

		// Initialization helpers
		void CreateGeometryBuffers ();
		GLuint CreateShader ( std::string const & filePath, GLenum type );
		void CreateShaderProgram ();
		void CreateVertexArray ();

		Application const * application;
		VoxelWorld * world;

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

		std::unordered_map <Voxel const *, unsigned int> voxelTransformIndices;
		GLuint transformBuffer;
		
						// Texture
		std::unordered_map <GLuint, std::unordered_map <Sides, Batch> >batches;
		MapBuffer <Voxel *, VoxelData> voxelBuffer;
		
	};
}