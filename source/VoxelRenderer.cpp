#include "VoxelRenderer.hpp"

namespace vw
{
	VoxelRenderer::VoxelRenderer ()
		: destroyResources ( true )
	{
		CreateGeometryBuffers ();
		CreateVertexArray ();
		CreateShaderProgram ();
	}

	VoxelRenderer::VoxelRenderer ( VoxelRenderer && r )
	{
		operator = ( std::move ( r ) );
	}

	VoxelRenderer::~VoxelRenderer ()
	{
		if ( destroyResources )
		{
			glDeleteBuffers ( 1, &vertexBuffer );
			glDeleteBuffers ( 1, &indexBuffer );
			glDeleteProgram ( shaderProgram );
			glDeleteVertexArrays ( 1, &vertexArray );
		}
	}

	VoxelRenderer & VoxelRenderer::operator = ( VoxelRenderer && r )
	{
		destroyResources = std::exchange ( r.destroyResources, false );

		vertexBuffer = r.vertexBuffer;
		vertexArray = r.vertexArray;
		indexBuffer = r.indexBuffer;
		shaderProgram = r.shaderProgram;

		return *this;
	}

	void VoxelRenderer::AddVoxelType ( std::string const & name, std::string const & textureFilePath )
	{

	}

	void VoxelRenderer::DeleteVoxelType ( std::string const & name )
	{

	}

	void VoxelRenderer::AddVoxel ( glm::vec3 const & position, std::string const & type )
	{

	}

	void VoxelRenderer::DeleteVoxel ( glm::vec3 const & position )
	{

	}

	void VoxelRenderer::Render ()
	{

	}

	void VoxelRenderer::CreateGeometryBuffers ()
	{

	}

	GLuint VoxelRenderer::CreateShader ()
	{
		return 0;
	}

	void VoxelRenderer::CreateShaderProgram ()
	{

	}

	void VoxelRenderer::CreateVertexArray ()
	{

	}
}