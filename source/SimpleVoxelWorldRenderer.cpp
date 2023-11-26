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
		voxelMesh ( "model/Voxel.obj", glm::scale (glm::identity <glm::mat4> (), { 0.5f, 0.5f, 0.5f }) ),
		voxelOutlineMesh ( "model/Voxel.obj", glm::scale ( glm::identity <glm::mat4> (), { 0.5f, 0.5f, 0.5f } ), "Outline" ),
		application ( & application ),
		world ( & world )
	{
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
			glDeleteProgram ( shaderProgram );
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

		voxelOutlineMesh.Bind ();

		// Set color to black
		glUniform4f ( glGetUniformLocation ( simpleShaderProgram, "u_color" ), 0.0f, 0.0f, 0.0f, 1.0f );

		// Set model matrix
		glUniformMatrix4fv ( glGetUniformLocation ( simpleShaderProgram, "u_modelMatrix" ),
			1, GL_FALSE, glm::value_ptr ( transform ) );

		glDrawElements ( GL_TRIANGLES, voxelOutlineMesh.GetIndexCount (), GL_UNSIGNED_INT, nullptr);
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

		voxelBuffer.Clear ();
		voxelBuffer .PushBack ( voxelDatas );
		voxelCount = voxelDatas.size ();
	}

	void SimpleVoxelWorldRenderer::Render ()
	{
		glUseProgram ( shaderProgram );
		
		voxelMesh.Bind ();

		// Bind transform buffer
		glBindBufferBase ( GL_UNIFORM_BUFFER, 0, voxelBuffer.GetBuffer () );

		auto texture { application->voxelTypeTextures.at ( "Grass" ) };

		glActiveTexture ( GL_TEXTURE0 );
		glBindTexture ( GL_TEXTURE_2D, texture );
		glUniform1i ( glGetUniformLocation ( shaderProgram, "u_texture" ), 0 );

		glDrawElementsInstanced (
			GL_TRIANGLES,
			6 * 6,
			GL_UNSIGNED_INT,
			0,
			voxelBuffer.GetElementCount ()
		);

		// Render outline
		{
			auto targetVoxel { world->raycaster.GetTargetVoxel () };
			if ( targetVoxel != nullptr )
				RenderOutline ( targetVoxel->GetTransformMatrix () );
		}
	}

	void SimpleVoxelWorldRenderer::CreateShaderProgram ()
	{
		shaderProgram = vw::CreateShaderProgram ( "shader/SimpleVoxelShader.glsl.vert", "shader/SimpleVoxelShader.glsl.frag" );
		simpleShaderProgram = vw::CreateShaderProgram ( "shader/SimpleShader.glsl.vert", "shader/SimpleShader.glsl.frag" );
	}
}