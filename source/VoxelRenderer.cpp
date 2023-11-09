#include "VoxelRenderer.hpp"

#include "Camera.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "OBJ_Loader.h"

namespace vw
{
	VoxelRenderer::VoxelRenderer ()
		: destroyResources ( true )
	{
		CreateGeometryBuffers ();
		CreateVertexArray ();

		glBindVertexArray ( vertexArray );
		glBindVertexBuffer ( 0, vertexBuffer, 0, sizeof ( GLfloat ) * ( 3 + 2 ) );
		
		CreateShaderProgram ();

		modelMatricesUniformLocation = glGetUniformLocation ( shaderProgram, "u_modelMatrices" );
		viewMatrixUniformLocation = glGetUniformLocation ( shaderProgram, "u_viewMatrix" );
		projectionMatrixUniformLocation = glGetUniformLocation ( shaderProgram, "u_projectionMatrix" );
		colorUniformLocation = glGetUniformLocation ( shaderProgram, "u_color" );
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

	void VoxelRenderer::AddVoxelType ( std::string const & name, std::string const & texturePath )
	{
		GLuint texture;
		
		{
			assert ( std::filesystem::exists ( texturePath ) );

			stbi_set_flip_vertically_on_load ( 1 );

			int width, height;
			auto data { stbi_load ( texturePath.data (), &width, &height, nullptr, STBI_rgb_alpha ) };
		
			glGenTextures ( 1, &texture );
			glBindTexture ( GL_TEXTURE_2D, texture );
			glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );
			glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
			glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
			glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
			glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
			glBindTexture ( GL_TEXTURE_2D, 0 );
		}

		batches.insert ( { name, Batch { texture } } );
	}

	void VoxelRenderer::DeleteVoxelType ( std::string const & name )
	{
		batches.erase ( name );
	}

	void VoxelRenderer::AddVoxel ( glm::vec3 const & position, std::string const & type, glm::mat4 rotation )
	{
		auto batchIt { batches.find ( type ) };

		assert ( batchIt != batches.end () );

		glm::mat4 translation { glm::translate ( glm::identity <glm::mat4> (), position ) };
		
		batchIt->second.voxelTransforms.push_back ( translation );
	}

	void VoxelRenderer::DeleteVoxel ( glm::vec3 const & position )
	{
		//auto locationIt { voxelLocations.find ( position ) };

		//assert ( locationIt != voxelLocations.end () );

		//std::batches [ locationIt->second.batchName ].voxelTransforms;
		//
		//[locationIt->second.voxelIndex] ;
		//std::erase ( locationIt->second.voxelTransforms.begin () ;
	
	}

	void VoxelRenderer::Render ()
	{
		glUseProgram ( shaderProgram );
		glBindVertexArray ( vertexArray );
		glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, indexBuffer );

		for ( auto const & [name, batch] : batches )
		{
			glActiveTexture ( GL_TEXTURE0 );
			glBindTexture ( GL_TEXTURE_2D, batch.texture );

			glUniform1i ( glGetUniformLocation ( shaderProgram, "u_texture" ), 0 );

			glUniformMatrix4fv ( 
				modelMatricesUniformLocation,
				batch.voxelTransforms.size (), 
				GL_FALSE, 
				glm::value_ptr ( batch.voxelTransforms.front () )
			);

			glDrawElementsInstanced ( GL_TRIANGLES, 6 * 6, GL_UNSIGNED_INT, nullptr, batch.voxelTransforms.size () );
		}
	}

	void VoxelRenderer::CreateGeometryBuffers ()
	{
		std::vector <GLfloat> vertices;
		std::vector <GLuint> indices;

		objl::Loader loader;
		loader.LoadFile ( "model/Voxel.obj" );

		auto mesh { loader.LoadedMeshes.front () };

		for ( auto const & vertex : mesh.Vertices )
		{
			vertices.push_back ( vertex.Position.X );
			vertices.push_back ( vertex.Position.Y );
			vertices.push_back ( vertex.Position.Z );

			vertices.push_back ( vertex.TextureCoordinate.X );
			vertices.push_back ( vertex.TextureCoordinate.Y );
		}

		for ( auto const & index : mesh.Indices )
			indices.push_back ( index );

		glGenBuffers ( 1, &vertexBuffer );
		glBindBuffer ( GL_ARRAY_BUFFER, vertexBuffer );
		glBufferData ( GL_ARRAY_BUFFER, vertices.size () * sizeof (GLfloat), vertices.data (), GL_STATIC_DRAW);

		glGenBuffers ( 1, &indexBuffer );
		glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, indexBuffer );
		glBufferData ( GL_ELEMENT_ARRAY_BUFFER, indices.size () * sizeof (GLuint), indices.data (), GL_STATIC_DRAW);
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
		glVertexAttribBinding ( 0, 0 );

		glEnableVertexAttribArray ( 1 );
		glVertexAttribFormat ( 1, 2, GL_FLOAT, GL_FALSE, sizeof ( GLfloat ) * 3 );
		glVertexAttribBinding ( 1, 0 );
	}
}