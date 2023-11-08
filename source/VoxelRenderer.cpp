#include "VoxelRenderer.hpp"

#include "Camera.hpp"

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

		modelMatrixUniformLocation = glGetUniformLocation ( shaderProgram, "u_modelMatrix" );
		viewMatrixUniformLocation = glGetUniformLocation ( shaderProgram, "u_viewMatrix" );
		projectionMatrixUniformLocation = glGetUniformLocation ( shaderProgram, "u_projectionMatrix" );
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

	void VoxelRenderer::SetView ( glm::mat4 const & viewMatrix )
	{
		glUseProgram ( shaderProgram );
		glUniformMatrix4fv ( viewMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr ( viewMatrix ) );
	}

	void VoxelRenderer::SetProjection ( glm::mat4 const & projectionMatrix )
	{
		glUseProgram ( shaderProgram );
		glUniformMatrix4fv ( projectionMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr ( projectionMatrix ) );
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

	void VoxelRenderer::SetCamera ( Camera & camera )
	{
		camera.AddSignalListener ( "ViewUpdate", [this, &camera] () { SetView ( camera.GetViewMatrix () ); } );
		camera.AddSignalListener ( "ProjectionUpdate", [this, &camera] () { SetProjection ( camera.GetProjectionMatrix () ); } );
		SetView ( camera.GetViewMatrix () );
		SetProjection ( camera.GetProjectionMatrix () );
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
		glUniformMatrix4fv ( modelMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr ( glm::translate ( glm::identity <glm::mat4> (), { 5.0f, 0.0f, 0.0f } ) ) );
		glBindVertexArray ( vertexArray );
		glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, indexBuffer );
		glDrawElements ( GL_TRIANGLES, 6 * 6, GL_UNSIGNED_INT, nullptr );
	}

	void VoxelRenderer::CreateGeometryBuffers ()
	{
		std::vector <GLfloat> vertices
		{
			//	| POSITION | | NORMAL |
	// Near
	 -1, -1,  1,
	 -1,  1,  1,
	  1,  1,  1,
	  1, -1,  1,

	  // Far
	  -1, -1, -1,
	  -1,  1, -1,
	   1,  1, -1,
	   1, -1, -1,

	   // Left
	   -1, -1, -1,
	   -1,  1, -1,
	   -1,  1,  1,
	   -1, -1,  1,

	   // Right
		 1, -1, -1,
		 1,  1, -1,
		 1,  1,  1,
		 1, -1,  1,

		 // Up
		 -1,  1, -1,
		 -1,  1,  1,
		  1,  1,  1,
		  1,  1, -1,

		  // Down
		 -1, -1, -1,
		 -1, -1,  1,
		  1, -1,  1,
		  1, -1, -1,
		};

		std::vector <GLuint> indices
		{
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4,
			8, 9, 10, 10, 11, 8,
			12, 13, 14, 14, 15, 12,
			16, 17, 18, 18, 19, 16,
			20, 21, 22, 22, 23, 20
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