#include "VoxelRenderer.hpp"

namespace vw
{
	VoxelRenderer::VoxelRenderer ()
		: destroyResources ( true )
	{
		CreateGeometryBuffers ();
		CreateVertexArray ();

		glBindVertexArray ( vertexArray );
		glBindVertexBuffer ( 0, vertexBuffer, 0, sizeof ( GLfloat ) * 3 );
		
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
		glUseProgram ( shaderProgram );
		glBindVertexArray ( vertexArray );
		glDrawArrays ( GL_TRIANGLES, 0, 3 );
	}

	void VoxelRenderer::CreateGeometryBuffers ()
	{
		std::vector <GLfloat> vertices
		{
			-1, -1, 0,
			0, 1, 0,
			1, -1, 0
		};

		std::vector <GLuint> indices
		{
			0, 1, 2
		};

		glGenBuffers ( 1, &vertexBuffer );
		glBindBuffer ( GL_ARRAY_BUFFER, vertexBuffer );
		glBufferData ( GL_ARRAY_BUFFER, vertices.size () * sizeof ( GLfloat ), vertices.data (), GL_STATIC_DRAW );

		glGenBuffers ( 1, &indexBuffer );
		glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, indexBuffer );
		glBufferData ( GL_ELEMENT_ARRAY_BUFFER, indices.size () * sizeof ( GLuint ), indices.data (), GL_STATIC_DRAW );

	}

	GLuint VoxelRenderer::CreateShader ( std::string const & filePath, GLenum type )
	{
		auto shader { glCreateShader ( type ) };

		std::string data { ( std::ostringstream {} << std::ifstream { filePath }.rdbuf () ).str () };
		
		GLchar const * shaderData { data.data () };
		GLint length { static_cast <GLint> ( data.size () ) };

		glShaderSource ( shader, 1, &shaderData, &length );
		glCompileShader ( shader );
		return shader;
	}

	void VoxelRenderer::CreateShaderProgram ()
	{
		GLuint vertexShader { CreateShader ( "shader/VoxelShader.glsl.vert", GL_VERTEX_SHADER ) };
		GLuint fragmentShader { CreateShader ( "shader/VoxelShader.glsl.frag", GL_FRAGMENT_SHADER ) };

		shaderProgram = glCreateProgram ();
		glAttachShader ( shaderProgram, vertexShader );
		glAttachShader ( shaderProgram, fragmentShader );
		glLinkProgram ( shaderProgram );

		glDeleteShader ( vertexShader );
		glDeleteShader ( fragmentShader );
	}

	void VoxelRenderer::CreateVertexArray ()
	{
		glGenVertexArrays ( 1, &vertexArray );
		glBindVertexArray ( vertexArray );
		glEnableVertexAttribArray ( 0 );
		glVertexAttribFormat ( 0, 3, GL_FLOAT, GL_FALSE, 0 );
	}
}