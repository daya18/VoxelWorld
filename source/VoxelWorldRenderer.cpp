#include "VoxelWorldRenderer.hpp"

#include "Camera.hpp"
#include "VoxelWorld.hpp"
#include "Utility.hpp"
#include "Application.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "OBJ_Loader.h"

namespace vw
{
	VoxelWorldRenderer::VoxelWorldRenderer ( Application & application, VoxelWorld & world )
	:
		destroyResources ( true ),
		application ( & application ),
		world ( & world )
	{
		CreateGeometryBuffers ();
		CreateVertexArray ();

		glBindVertexArray ( vertexArray );
		glBindVertexBuffer ( 0, vertexBuffer, 0, sizeof ( GLfloat ) * ( 3 + 2 ) );

		CreateShaderProgram ();

		auto transformsBI { glGetUniformBlockIndex ( shaderProgram, "u_Transforms" ) };
		glUniformBlockBinding ( shaderProgram, transformsBI, 0 );

		auto transformIndicesBI { glGetUniformBlockIndex ( shaderProgram, "u_TransformIndices" ) };
		glUniformBlockBinding ( shaderProgram, transformIndicesBI, 1 );

		modelMatricesUniformLocation = glGetUniformLocation ( shaderProgram, "u_modelMatrices" );
		viewMatrixUniformLocation = glGetUniformLocation ( shaderProgram, "u_viewMatrix" );
		projectionMatrixUniformLocation = glGetUniformLocation ( shaderProgram, "u_projectionMatrix" );
		colorUniformLocation = glGetUniformLocation ( shaderProgram, "u_color" );
	}

	VoxelWorldRenderer::VoxelWorldRenderer ( VoxelWorldRenderer && r )
	{
		operator = ( std::move ( r ) );
	}

	VoxelWorldRenderer::~VoxelWorldRenderer ()
	{
		if ( destroyResources )
		{
			glDeleteBuffers ( 1, &vertexBuffer );
			glDeleteBuffers ( 1, &indexBuffer );
			glDeleteProgram ( shaderProgram );
			glDeleteVertexArrays ( 1, &vertexArray );
		}
	}

	void VoxelWorldRenderer::SetView ( glm::mat4 const & viewMatrix )
	{
		glUseProgram ( shaderProgram );
		glUniformMatrix4fv ( viewMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr ( viewMatrix ) );
	}

	void VoxelWorldRenderer::SetProjection ( glm::mat4 const & projectionMatrix )
	{
		glUseProgram ( shaderProgram );
		glUniformMatrix4fv ( projectionMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr ( projectionMatrix ) );
	}

	VoxelWorldRenderer & VoxelWorldRenderer::operator = ( VoxelWorldRenderer && r )
	{
		destroyResources = std::exchange ( r.destroyResources, false );

		vertexBuffer = r.vertexBuffer;
		vertexArray = r.vertexArray;
		indexBuffer = r.indexBuffer;
		shaderProgram = r.shaderProgram;

		std::cout << "Warning: Move semantics not fully implemented" << std::endl;

		return *this;
	}

	void VoxelWorldRenderer::SetCamera ( Camera & camera )
	{
		camera.AddSignalListener ( "ViewUpdate", [this, &camera] () { SetView ( camera.GetViewMatrix () ); } );
		camera.AddSignalListener ( "ProjectionUpdate", [this, &camera] () { SetProjection ( camera.GetProjectionMatrix () ); } );
		SetView ( camera.GetViewMatrix () );
		SetProjection ( camera.GetProjectionMatrix () );
	}
	
	void VoxelWorldRenderer::Update ()
	{
		std::vector <glm::mat4> voxelTransforms;
		voxelTransforms.reserve ( world->voxels.size () );

		for ( auto & [ voxelPos, voxel ] : world->voxels )
		{
			auto translation { glm::vec3 { voxelPos } * voxelScale * 2.0f };
			glm::mat4 translationMat { glm::translate ( glm::identity <glm::mat4> (), translation ) };
			glm::mat4 scaleMat { glm::scale ( glm::identity <glm::mat4> (), glm::vec3 ( voxelScale, voxelScale, voxelScale ) ) };
			voxelTransforms.push_back ( translationMat * scaleMat );
		}

		transformBuffer = CreateBufferWithData ( voxelTransforms, GL_UNIFORM_BUFFER );

		batches.clear ();

		int voxelIndex { 0 };
		for ( auto & [ voxelPos, voxel ] : world->voxels )
		{
			for ( auto const & side : Voxel::sides )
			{
				if ( voxel.CheckNeighbour ( side ) )
					continue;

				batches [ application->voxelTypeTextures.at ( voxel.GetType () ) ]
					[ side ].transformIndexBuffer.PushBack ( { glm::vec4 { static_cast < float > ( voxelIndex ) } } );
			}

			++voxelIndex;
		}
	}

	void VoxelWorldRenderer::Render ()
	{
		glUseProgram ( shaderProgram );

		// Bind vertex and index buffers
		glBindVertexArray ( vertexArray );
		glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, indexBuffer );

		// Bind transform buffer
		glBindBufferBase ( GL_UNIFORM_BUFFER, 0, transformBuffer );

		for ( auto const & [texture, faceBatches] : batches )
		{
			for ( auto const & [face, batch] : faceBatches )
			{
				glActiveTexture ( GL_TEXTURE0 );
				glBindTexture ( GL_TEXTURE_2D, texture );
				glUniform1i ( glGetUniformLocation ( shaderProgram, "u_texture" ), 0 );

				glBindBufferBase ( GL_UNIFORM_BUFFER, 1, batch.transformIndexBuffer.Get () );

				glDrawElementsInstanced (
					GL_TRIANGLES,
					6,
					GL_UNSIGNED_INT,
					reinterpret_cast < void * > ( GetFaceIndexOffset ( face ) ),
					batch.transformIndexBuffer.GetElementCount () );
			}
		}
	}

	void VoxelWorldRenderer::CreateGeometryBuffers ()
	{

		std::vector <GLfloat> vertices;
		std::vector <GLuint> indices;

		objl::Loader loader;
		loader.LoadFile ( "model/Voxel.obj" );

		auto indexOffset { 0 };

		for ( auto const & mesh : loader.LoadedMeshes )
		{
			for ( auto const & vertex : mesh.Vertices )
			{
				vertices.push_back ( vertex.Position.X );
				vertices.push_back ( vertex.Position.Y );
				vertices.push_back ( vertex.Position.Z );

				vertices.push_back ( vertex.TextureCoordinate.X );
				vertices.push_back ( vertex.TextureCoordinate.Y );
			}

			for ( auto const & index : mesh.Indices )
			{
				indices.push_back ( indexOffset + index );
			}

			indexOffset += mesh.Vertices.size ();
		}

		glGenBuffers ( 1, &vertexBuffer );
		glBindBuffer ( GL_ARRAY_BUFFER, vertexBuffer );
		glBufferData ( GL_ARRAY_BUFFER, vertices.size () * sizeof ( GLfloat ), vertices.data (), GL_STATIC_DRAW );

		glGenBuffers ( 1, &indexBuffer );
		glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, indexBuffer );
		glBufferData ( GL_ELEMENT_ARRAY_BUFFER, indices.size () * sizeof ( GLuint ), indices.data (), GL_STATIC_DRAW );
	}

	GLuint VoxelWorldRenderer::CreateShader ( std::string const & filePath, GLenum type )
	{
		auto shader { glCreateShader ( type ) };

		std::string data { ( std::ostringstream {} << std::ifstream { filePath }.rdbuf () ).str () };

		GLchar const * shaderData { data.data () };
		GLint length { static_cast < GLint > ( data.size () ) };

		glShaderSource ( shader, 1, &shaderData, &length );
		glCompileShader ( shader );
		return shader;
	}

	void VoxelWorldRenderer::CreateShaderProgram ()
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

	void VoxelWorldRenderer::CreateVertexArray ()
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

	GLuint VoxelWorldRenderer::GetFaceIndexOffset ( Voxel::Sides face )
	{
		// TO DO: Hard coded values, bad...
		switch ( face )
		{
		case Voxel::Sides::back:	return ( 6 * 0 ) * sizeof ( GLuint );
		case Voxel::Sides::left:	return ( 6 * 1 ) * sizeof ( GLuint );
		case Voxel::Sides::forward:	return ( 6 * 2 ) * sizeof ( GLuint );
		case Voxel::Sides::up:		return ( 6 * 3 ) * sizeof ( GLuint );
		case Voxel::Sides::down:	return ( 6 * 4 ) * sizeof ( GLuint );
		case Voxel::Sides::right:	return ( 6 * 5 ) * sizeof ( GLuint );
		}
	}
}