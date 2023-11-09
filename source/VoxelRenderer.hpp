#pragma once

namespace vw
{
	class Camera;

	class VoxelRenderer
	{
	public:
		VoxelRenderer ();
		VoxelRenderer ( VoxelRenderer const & ) = delete;
		VoxelRenderer ( VoxelRenderer && );
		~VoxelRenderer ();

		VoxelRenderer & operator = ( VoxelRenderer const & ) = delete;
		VoxelRenderer & operator = ( VoxelRenderer && );

		void AddVoxelType ( std::string const & name, std::string const & texturePath );
		void DeleteVoxelType ( std::string const & name );

		void SetCamera ( Camera & );

		void AddVoxel ( glm::vec3 const & position, std::string const & type, glm::mat4 rotation = glm::identity <glm::mat4> () );
		void DeleteVoxel ( glm::vec3 const & position );

		void Render ();

	private:
		struct Batch
		{
			GLuint texture;
			std::vector <glm::mat4> voxelTransforms;
		};

		struct VoxelLocation
		{
			std::string batchName;
			unsigned int voxelIndex;
		};

		void SetView ( glm::mat4 const & );
		void SetProjection ( glm::mat4 const & );

		void CreateGeometryBuffers ();
		GLuint CreateShader ( std::string const & filePath, GLenum type );
		void CreateShaderProgram ();
		void CreateVertexArray ();

		bool destroyResources;
		GLuint vertexBuffer;
		GLuint indexBuffer;
		GLuint shaderProgram;
		GLuint vertexArray;
		//GLuint transformBuffer;
		std::vector <GLuint> voxelTypeTextures;

		GLuint modelMatricesUniformLocation;
		GLuint viewMatrixUniformLocation;
		GLuint projectionMatrixUniformLocation;
		GLuint colorUniformLocation;
		
		std::unordered_map <std::string, Batch> batches;
		//std::unordered_map <glm::vec3, VoxelLocation> voxelLocations;
	};
}