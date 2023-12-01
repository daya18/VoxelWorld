#include "VoxelWorld.hpp"

#include "Window.hpp"
#include "Application.hpp"
#include "Core.hpp"

namespace vw
{
	VoxelWorld::VoxelWorld ( Application & application )
	: 
		application ( &application ),
		window ( & application.window ),
		raycaster ( application, *this ),
		voxelMesh ( "model/Voxel.obj", glm::scale ( glm::identity <glm::mat4> (), { 0.5f, 0.5f, 0.5f } ) ),
		voxelOutlineMesh ( "model/Voxel.obj", glm::scale ( glm::identity <glm::mat4> (), { 0.5f, 0.5f, 0.5f } ), "Outline" ),
		shaderProgram ( "shader/SimpleVoxelShader" ),
		simpleShaderProgram ( "shader/SimpleShader" )
	{
	}

	void VoxelWorld::AddVoxel ( glm::vec3 const & position, std::string const & type )
	{
		auto & voxel { voxels.insert ( { position, { *this, { position, type } } } ).first->second };
		
		voxel.FindNeighbours ();

		for ( auto neighbour : voxel.GetNeighbours () )
			neighbour->FindNeighbours ();

		typeVoxelBuffers[type].Insert ({ { &voxel, { voxel.GetTransformMatrix () } } });

		for ( auto neighbour : voxel.GetNeighbours () )
			if ( neighbour->GetNeighbours ().size () == 6 )
				typeVoxelBuffers[neighbour->GetType()].Erase ( { neighbour } );

	}	
	
	void VoxelWorld::Fill ( glm::vec3 const & fromPosition, glm::vec3 const & toPosition, std::string const & voxelType )
	{
		auto min { glm::min ( fromPosition, toPosition ) };
		auto max { glm::max ( fromPosition, toPosition ) };

		// TO DO: Optimize using std::vector::reserve

		std::vector <Voxel *> visibleVoxels;
		
		for ( float x = min.x; x <= max.x; ++x )
		{
			for ( float y = min.y; y <= max.y; ++y )
			{
				for ( float z = min.z; z <= max.z; ++z )
				{
					glm::vec3 position { x, y, z };

					auto insertResult { voxels.insert ( { position, { *this, { position, voxelType } } } ) };

					if ( x == min.x || x == max.x || y == min.y || y == max.y || z == min.z || z == max.z )
						visibleVoxels.push_back ( &insertResult.first->second );
				}
			}
		}

		// TO DO: Update only affected voxels
		for ( auto & [position, voxel] : voxels )
			voxel.FindNeighbours ();

		std::vector <std::pair <Voxel *, VoxelData>> voxelDatas;
		voxelDatas.reserve ( visibleVoxels.size () );

		for ( auto voxel : visibleVoxels )
			voxelDatas.push_back ( { voxel, { voxel->GetTransformMatrix () } } );

		typeVoxelBuffers[voxelType].Insert (voxelDatas);
	}

	void VoxelWorld::RemoveVoxel ( Voxel * voxel )
	{
		auto voxelType { voxel->GetType () };
		auto neighbours { voxel->GetNeighbours () };
			
		this->voxels.erase ( voxel->GetPosition () );
			
		if ( voxel == raycaster.GetTargetVoxel () )
			raycaster.Clear ();
		
		for ( auto & neighbour : neighbours )
			neighbour->FindNeighbours ();
		
		typeVoxelBuffers[voxelType].Erase ( { voxel } );

		for ( auto neighbour : neighbours )
			if ( neighbour->GetNeighbours ().size () == 5 )
				typeVoxelBuffers [ neighbour->GetType () ].Insert ( { { neighbour, { neighbour->GetTransformMatrix () } } });
	}

	void VoxelWorld::SetCamera ( Camera & camera )
	{
		this->camera = &camera;
	}

	void VoxelWorld::Update ( float deltaTime )
	{
		if ( ! application->CheckPaused () )
		{
			raycaster.Update ();
		}
	}

	void VoxelWorld::Render ()
	{
		glEnable ( GL_DEPTH_TEST );

		glEnable ( GL_CULL_FACE );
		glCullFace ( GL_BACK );
		glFrontFace ( GL_CCW );

		shaderProgram.Use ();
		camera->Bind ( shaderProgram );
		voxelMesh.Bind ();

		for ( auto const & [type, voxelBuffer] : typeVoxelBuffers )
		{
			// Bind transform buffer
			glBindBufferBase ( GL_UNIFORM_BUFFER, 0, voxelBuffer.GetValueBuffer ().GetBuffer () );

			auto texture { application->voxelTypeTextures.at ( type ) };

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
		}

		// Render outline
		{
			auto targetVoxel { raycaster.GetTargetVoxel () };
			if ( targetVoxel != nullptr )
				RenderVoxelOutline ( targetVoxel->GetTransformMatrix () );
		}

		glDisable ( GL_CULL_FACE );
		glDisable ( GL_DEPTH_TEST );
	}
	
	void VoxelWorld::RenderVoxelOutline ( glm::mat4 const & transform )
	{
		simpleShaderProgram.Use ();

		camera->Bind ( simpleShaderProgram );

		voxelOutlineMesh.Bind ();

		// Set color to black
		simpleShaderProgram.SetUniform ( "u_color", { 0.0f, 0.0f, 0.0f, 1.0f } );

		// Set model matrix
		simpleShaderProgram.SetUniform ( "u_modelMatrix", transform );

		glDrawElements ( GL_TRIANGLES, voxelOutlineMesh.GetIndexCount (), GL_UNSIGNED_INT, nullptr );
	}

	Voxel * VoxelWorld::FindVoxel ( glm::vec3 const & position )
	{
		auto voxelIt { voxels.find ( position ) };
		if ( voxelIt == voxels.end () ) return nullptr;
		return &voxelIt->second;
	}

	std::vector <Voxel *> VoxelWorld::GetVoxels ()
	{
		std::vector <Voxel *> outVoxels;
		outVoxels.reserve ( voxels.size () );

		for ( auto & [ pos, vox ] : voxels )
			outVoxels.push_back ( &vox );
		
		return outVoxels;
	}

	std::vector <Voxel *> VoxelWorld::GetVoxels ( glm::vec3 const & centre, float radius )
	{
		auto diameter { radius * 2.0f };

		std::vector < Voxel *> voxels;
		voxels.reserve ( this->voxels.size () );

		glm::vec3 begin { centre - radius };
		glm::vec3 end { centre + radius };

		
		for ( float x { begin.x }; x <= end.x; x += 1.0f )
		{
			for ( float y { begin.y }; y <= end.y; y += 1.0f )
			{
				for ( float z { begin.z }; z <= end.z; z += 1.0f )
				{
					auto voxelIt { this->voxels.find ( glm::vec3 { x, y, z } ) };
					
					if ( voxelIt != this->voxels.end () )
						voxels.push_back ( &voxelIt->second );
				}
			}
		}			
		
		return voxels;
	}
}