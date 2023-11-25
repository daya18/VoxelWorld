#include "SimpleVoxelWorldRenderer.hpp"

#include "Camera.hpp"
#include "VoxelWorld.hpp"
#include "core.hpp"
#include "Application.hpp"

#include "stb_image.h"

#include "impex.hpp"

namespace vw
{
	SimpleVoxelWorldRenderer::SimpleVoxelWorldRenderer ( Application & application, VoxelWorld & world )
		:
		destroyResources ( true ),
		application ( & application ),
		world ( & world )
	{
		CreateGeometryBuffers ();
		CreateVertexArray ();

		//glBindVertexArray ( vertexArray );
		//glBindVertexBuffer ( 0, vertexBuffer, 0, sizeof ( GLfloat ) * ( 3 + 2 + 3 ) );

		CreateShaderProgram ();

		auto transformsBI { glGetUniformBlockIndex ( shaderProgram, "u_Transforms" ) };
		glUniformBlockBinding ( shaderProgram, transformsBI, 0 );

		modelMatricesUniformLocation = glGetUniformLocation ( shaderProgram, "u_modelMatrices" );
		viewMatrixUniformLocation = glGetUniformLocation ( shaderProgram, "u_viewMatrix" );
		projectionMatrixUniformLocation = glGetUniformLocation ( shaderProgram, "u_projectionMatrix" );
		colorUniformLocation = glGetUniformLocation ( shaderProgram, "u_color" );
	}

	SimpleVoxelWorldRenderer::SimpleVoxelWorldRenderer ( SimpleVoxelWorldRenderer && r )
	{
		operator = ( std::move ( r ) );
	}

	SimpleVoxelWorldRenderer::~SimpleVoxelWorldRenderer ()
	{
		if ( destroyResources )
		{
			glDeleteBuffers ( 1, &vertexBuffer );
			glDeleteBuffers ( 1, &indexBuffer );
			glDeleteProgram ( shaderProgram );
			glDeleteVertexArrays ( 1, &vertexArray );
		}
	}

	void SimpleVoxelWorldRenderer::SetView ( glm::mat4 const & viewMatrix )
	{
		glUseProgram ( shaderProgram );
		glUniformMatrix4fv ( viewMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr ( viewMatrix ) );
		glUseProgram ( simpleShaderProgram );
		glUniformMatrix4fv ( viewMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr ( viewMatrix ) );

	}

	void SimpleVoxelWorldRenderer::SetProjection ( glm::mat4 const & projectionMatrix )
	{
		glUseProgram ( shaderProgram );
		glUniformMatrix4fv ( projectionMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr ( projectionMatrix ) );
		glUseProgram ( simpleShaderProgram );
		glUniformMatrix4fv ( projectionMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr ( projectionMatrix ) );
	}

	SimpleVoxelWorldRenderer & SimpleVoxelWorldRenderer::operator = ( SimpleVoxelWorldRenderer && r )
	{
		destroyResources = std::exchange ( r.destroyResources, false );

		vertexBuffer = r.vertexBuffer;
		vertexArray = r.vertexArray;
		indexBuffer = r.indexBuffer;
		shaderProgram = r.shaderProgram;

		std::cout << "Warning: Move semantics not fully implemented" << std::endl;

		return *this;
	}

	void SimpleVoxelWorldRenderer::SetCamera ( Camera & camera )
	{
		camera.AddSignalListener ( "ViewUpdate", [this, &camera] () { SetView ( camera.GetViewMatrix () ); } );
		camera.AddSignalListener ( "ProjectionUpdate", [this, &camera] () { SetProjection ( camera.GetProjectionMatrix () ); } );
		SetView ( camera.GetViewMatrix () );
		SetProjection ( camera.GetProjectionMatrix () );
	}

	void SimpleVoxelWorldRenderer::AddVoxels ( std::vector <Voxel *> const & newVoxels )
	{
		Update ();
	}

	void SimpleVoxelWorldRenderer::RemoveVoxels ( std::vector <Voxel *> const & voxels )
	{
		Update ();
	}
	
	void SimpleVoxelWorldRenderer::RenderOutline ( glm::mat4 const & transform )
	{
		glUseProgram ( simpleShaderProgram );

		// Bind vertex and index buffers
		glBindVertexArray ( vertexArray );
		glBindVertexBuffer ( 0, outlineVertexBuffer, 0, sizeof ( GLfloat ) * ( 3 + 2 + 3 ) );
		glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, outlineIndexBuffer );

		// Set color to black
		glUniform4f ( glGetUniformLocation ( simpleShaderProgram, "u_color" ), 0.0f, 0.0f, 0.0f, 1.0f );

		// Set model matrix
		glUniformMatrix4fv ( glGetUniformLocation ( simpleShaderProgram, "u_modelMatrix" ),
			1, GL_FALSE, glm::value_ptr ( transform ) );

		glDrawElements ( GL_TRIANGLES, outlineIndexCount, GL_UNSIGNED_INT, nullptr );
	}

	void SimpleVoxelWorldRenderer::Update ()
	{
		std::vector <VoxelData> voxelDatas;

		voxelDatas.reserve ( world->voxels.size () );

		for ( auto const & [position, voxel] : world->voxels )
		{
			if ( voxel.GetNeighbours ().size () == 6 )
				continue;

			voxelDatas.push_back ( { voxel.GetTransformMatrix () } );
		}

		glDeleteBuffers ( 1, &voxelBuffer );
		voxelBuffer = CreateBufferWithData <VoxelData> ( voxelDatas, GL_UNIFORM_BUFFER );
		voxelCount = voxelDatas.size ();
	}

	void SimpleVoxelWorldRenderer::Render ()
	{
		glUseProgram ( shaderProgram );

		// Bind vertex and index buffers
		glBindVertexArray ( vertexArray );
		glBindVertexBuffer ( 0, vertexBuffer, 0, sizeof ( GLfloat ) * ( 3 + 2 + 3 ) );
		glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, indexBuffer );

		// Bind transform buffer
		glBindBufferBase ( GL_UNIFORM_BUFFER, 0, voxelBuffer );

		auto texture { application->voxelTypeTextures.at ( "Grass" ) };

		glActiveTexture ( GL_TEXTURE0 );
		glBindTexture ( GL_TEXTURE_2D, texture );
		glUniform1i ( glGetUniformLocation ( shaderProgram, "u_texture" ), 0 );

		glDrawElementsInstanced (
			GL_TRIANGLES,
			6 * 6,
			GL_UNSIGNED_INT,
			0,
			voxelCount 
		);

		// Render outline
		{
			auto targetVoxel { world->raycaster.GetTargetVoxel () };
			if ( targetVoxel != nullptr )
				RenderOutline ( targetVoxel->GetTransformMatrix () );
		}
	}

	void SimpleVoxelWorldRenderer::CreateGeometryBuffers ()
	{
		auto voxelModel { application->voxelModel };
		voxelModel.Transform ( glm::scale ( glm::identity <glm::mat4> (), { 0.5f, 0.5f, 0.5f } ) );

		auto voxelMesh { voxelModel.GetScene () };
		vertexBuffer = CreateBufferWithData ( impex::InterleaveVertices ( voxelMesh.vertices ), GL_ARRAY_BUFFER );
		indexBuffer = CreateBufferWithData ( voxelMesh.indices, GL_ELEMENT_ARRAY_BUFFER );

		auto outlineMesh { voxelModel.GetScene ().meshes.at ( "Outline" ) };
		outlineVertexBuffer = CreateBufferWithData ( impex::InterleaveVertices ( outlineMesh.vertices ), GL_ARRAY_BUFFER );
		outlineIndexBuffer = CreateBufferWithData ( outlineMesh.indices, GL_ELEMENT_ARRAY_BUFFER );
		outlineIndexCount = outlineMesh.indices.size ();
	}

	void SimpleVoxelWorldRenderer::CreateShaderProgram ()
	{
		shaderProgram = vw::CreateShaderProgram (
			"shader/SimpleVoxelShader.glsl.vert",
			"shader/SimpleVoxelShader.glsl.frag" );

		simpleShaderProgram = vw::CreateShaderProgram (
			"shader/SimpleShader.glsl.vert",
			"shader/SimpleShader.glsl.frag" );
	}


	void SimpleVoxelWorldRenderer::CreateVertexArray ()
	{
		glGenVertexArrays ( 1, &vertexArray );
		glBindVertexArray ( vertexArray );

		glEnableVertexAttribArray ( 0 );
		glVertexAttribFormat ( 0, 3, GL_FLOAT, GL_FALSE, 0 );
		glVertexAttribBinding ( 0, 0 );

		glEnableVertexAttribArray ( 1 );
		glVertexAttribFormat ( 1, 2, GL_FLOAT, GL_FALSE, sizeof ( GLfloat ) * 3 );
		glVertexAttribBinding ( 1, 0 );

		glEnableVertexAttribArray ( 2 );
		glVertexAttribFormat ( 2, 3, GL_FLOAT, GL_FALSE, sizeof ( GLfloat ) * ( 3 + 2 ) );
		glVertexAttribBinding ( 2, 0 );
	}

	GLuint SimpleVoxelWorldRenderer::GetFaceIndexOffset ( Sides face )
	{
		// TO DO: Hard coded values, bad...
		switch ( face )
		{
		case Sides::back:	return ( 6 * 0 ) * sizeof ( GLuint );
		case Sides::left:	return ( 6 * 1 ) * sizeof ( GLuint );
		case Sides::forward:	return ( 6 * 2 ) * sizeof ( GLuint );
		case Sides::up:		return ( 6 * 3 ) * sizeof ( GLuint );
		case Sides::down:	return ( 6 * 4 ) * sizeof ( GLuint );
		case Sides::right:	return ( 6 * 5 ) * sizeof ( GLuint );
		default:					return 0;
		}
	}
}