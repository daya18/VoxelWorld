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

		void AddVoxelType ( std::string const & name, std::string const & textureFilePath );
		void DeleteVoxelType ( std::string const & name );

		void SetCamera ( Camera & );

		void AddVoxel ( glm::vec3 const & position, std::string const & type );
		void DeleteVoxel ( glm::vec3 const & position );

		void Render ();

	private:
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
		std::vector <GLuint> voxelTypeTextures;
		GLuint viewMatrixUniformLocation;
		GLuint projectionMatrixUniformLocation;
	};
}