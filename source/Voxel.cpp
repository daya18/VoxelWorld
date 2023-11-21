#include "Voxel.hpp"

#include "VoxelWorld.hpp"
#include "VoxelWorld.hpp"
#include "Application.hpp"
#include "Utility.hpp"

namespace vw
{
	Voxel::Voxel ( VoxelWorld & world, Definition const & definition )
	: 
		world ( & world ),
		position ( definition.position ),
		type ( definition.type ),
		transform ( glm::translate ( glm::identity <glm::mat4> (), position ) ),
		worldSpaceModel ( world.GetApplication ().GetVoxelModel () )
	{
		glm::mat4 scaleMatrix { glm::scale ( glm::identity <glm::mat4> (), { 0.5f, 0.5f, 0.5f } ) };
		worldSpaceModel.Transform ( transform * scaleMatrix );
	}

	bool Voxel::CheckNeighbour ( Sides side ) const
	{
		return neighbours [ static_cast < int > ( side ) ] != nullptr;
	}

	bool Voxel::CheckFaceIntersection ( Sides side, float & distance ) const
	{
		auto & faceMesh { worldSpaceModel.GetFaceMesh ( side ) };

		for ( int triangleIndex { 0 }; triangleIndex < 2; ++triangleIndex )
		{
			int baseIndex { triangleIndex * 3 };

			glm::vec2 intersectionBaryPosition;
			float intersectionDistance;

			bool intersects {
				glm::intersectRayTriangle (
					world->camera.GetPosition (),
					world->camera.GetTargetDirection (),
					faceMesh.vertices [ faceMesh.indices [ baseIndex + 0 ] ].position,
					faceMesh.vertices [ faceMesh.indices [ baseIndex + 1 ] ].position,
					faceMesh.vertices [ faceMesh.indices [ baseIndex + 2 ] ].position,
					intersectionBaryPosition, intersectionDistance )
			};

			if ( intersects )
			{
				distance = intersectionDistance;
				return true;
			}
		}

		return false;
	}

	void Voxel::ClearNeighbours ()
	{
		for ( auto & neighbour : neighbours )
			neighbour = nullptr;
	}

	void Voxel::SetNeighbour ( Sides side, Voxel & neighbour )
	{
		neighbours [ static_cast < int > ( side ) ] = &neighbour;
	}
}
