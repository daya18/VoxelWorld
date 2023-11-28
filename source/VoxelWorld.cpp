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
		camera ( *window ),
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
		
		UpdateVoxelNeighbours ( voxel );

		for ( auto neighbour : voxel.GetNeighbours () )
			UpdateVoxelNeighbours ( *neighbour );

		UpdateRenderList ();
	}	
	
	void VoxelWorld::Fill ( glm::vec3 const & fromPosition, glm::vec3 const & toPosition, std::string const & voxelType )
	{
		auto min { glm::min ( fromPosition, toPosition ) };
		auto max { glm::max ( fromPosition, toPosition ) };

		// TO DO: Optimize using std::vector::reserve

		std::vector <Voxel *> newVoxels;
		
		for ( float x = min.x; x <= max.x; ++x )
		{
			for ( float y = min.y; y <= max.y; ++y )
			{
				for ( float z = min.z; z <= max.z; ++z )
				{
					glm::vec3 position { x, y, z };
					newVoxels.push_back ( &voxels.insert ( { position, { *this, { position, voxelType } } } ).first->second );
				}
			}
		}

		UpdateAllVoxelNeighbours ();

		UpdateRenderList ();
	}

	void VoxelWorld::RemoveVoxels ( std::vector <Voxel *> const & voxels )
	{
		std::vector <Voxel *> neighbours;
		neighbours.reserve ( voxels.size () * 6 );

		for ( auto const & voxel : voxels )
		{
			auto voxelNeighbours { voxel->GetNeighbours () };
			neighbours.insert ( neighbours.end (), voxelNeighbours.begin (), voxelNeighbours.end () );
			
			this->voxels.erase ( voxel->GetPosition () );
			
			if ( voxel == raycaster.GetTargetVoxel () )
				raycaster.Clear ();
		}
		
		for ( auto & voxel : neighbours )
				UpdateVoxelNeighbours ( *voxel );
		
		UpdateRenderList ();
	}

	void VoxelWorld::Update ( float deltaTime )
	{
		camera.Update ( deltaTime );
		raycaster.Update ();
	}

	void VoxelWorld::Render ()
	{
		shaderProgram.Use ();
		camera.Bind ( shaderProgram );
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
			auto targetVoxel { raycaster.GetTargetVoxel () };
			if ( targetVoxel != nullptr )
				RenderVoxelOutline ( targetVoxel->GetTransformMatrix () );
		}
	}
	
	void VoxelWorld::UpdateAllVoxelNeighbours ()
	{
		for ( auto & [position, voxel] : voxels )
			UpdateVoxelNeighbours ( voxel );
	}

	void VoxelWorld::UpdateVoxelNeighbours ( Voxel & voxel )
	{
		voxel.neighbours = {
			FindVoxel ( voxel.GetPosition () + GetDirectionVector ( Sides::up ) ),
			FindVoxel ( voxel.GetPosition () + GetDirectionVector ( Sides::down ) ),
			FindVoxel ( voxel.GetPosition () + GetDirectionVector ( Sides::left ) ),
			FindVoxel ( voxel.GetPosition () + GetDirectionVector ( Sides::right ) ),
			FindVoxel ( voxel.GetPosition () + GetDirectionVector ( Sides::forward ) ),
			FindVoxel ( voxel.GetPosition () + GetDirectionVector ( Sides::back ) ),
		};
	}

	void VoxelWorld::UpdateRenderList ()
	{
		std::vector <VoxelData> voxelDatas;

		voxelDatas.reserve ( voxels.size () );

		for ( auto const & [position, voxel] : voxels )
		{
			if ( voxel.GetNeighbours ().size () == 6 )
				continue;

			voxelDatas.push_back ( { voxel.GetTransformMatrix () } );
		}

		voxelBuffer.Clear ();
		voxelBuffer.PushBack ( voxelDatas );
	}

	void VoxelWorld::RenderVoxelOutline ( glm::mat4 const & transform )
	{
		simpleShaderProgram.Use ();

		camera.Bind ( simpleShaderProgram );

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