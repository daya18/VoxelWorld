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
		world ( & world ),
		voxelMesh ( "model/Voxel.obj", glm::scale (glm::identity <glm::mat4> (), { 0.5f, 0.5f, 0.5f }) ),
		voxelOutlineMesh ( "model/Voxel.obj", glm::scale ( glm::identity <glm::mat4> (), { 0.5f, 0.5f, 0.5f } ), "Outline" ),
		shaderProgram ( "shader/SimpleVoxelShader" ),
		simpleShaderProgram ( "shader/SimpleShader" )
	{
	}

	void SimpleVoxelWorldRenderer::SetView ( glm::mat4 const & viewMatrix )
	{
		shaderProgram.Use ();
		shaderProgram.SetUniform ( "u_viewMatrix", viewMatrix );
		simpleShaderProgram.Use ();
		shaderProgram.SetUniform ( "u_viewMatrix", viewMatrix );
	}

	void SimpleVoxelWorldRenderer::SetProjection ( glm::mat4 const & projectionMatrix )
	{
		shaderProgram.Use ();
		shaderProgram.SetUniform ( "u_projectionMatrix", projectionMatrix );
		simpleShaderProgram.Use ();
		shaderProgram.SetUniform ( "u_projectionMatrix", projectionMatrix );
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
		simpleShaderProgram.Use ();

		voxelOutlineMesh.Bind ();

		// Set color to black
		simpleShaderProgram.SetUniform ( "u_color", { 0.0f, 0.0f, 0.0f, 1.0f } );

		// Set model matrix
		simpleShaderProgram.SetUniform ( "u_modelMatrix", transform );

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
		voxelCount = static_cast <GLuint> ( voxelDatas.size () );
	}

	void SimpleVoxelWorldRenderer::Render ()
	{
		shaderProgram.Use ();
		
		voxelMesh.Bind ();

		// Bind transform buffer
		glBindBufferBase ( GL_UNIFORM_BUFFER, 0, voxelBuffer.GetBuffer () );

		auto texture { application->voxelTypeTextures.at ( "Grass" ) };

		glActiveTexture ( GL_TEXTURE0 );
		glBindTexture ( GL_TEXTURE_2D, texture );
		shaderProgram.SetUniform ( "u_texture", 0 );

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
}