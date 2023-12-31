#include "Voxel.hpp"

#include "VoxelWorld.hpp"
#include "VoxelWorld.hpp"
#include "Application.hpp"
#include "core.hpp"

#include <ei/3dintersection.hpp>

namespace vw
{
	Voxel::Voxel ( VoxelWorld & world, Definition const & definition )
	:
		world ( & world ),
		position ( definition.position ),
		type ( definition.type ),
		transform ( glm::translate ( glm::identity <glm::mat4> (), position ) )
	{
		glm::vec3 min { transform * glm::vec4 { -0.5f, -0.5f, -0.5f, 1.0f } };
		glm::vec3 max { transform * glm::vec4 {  0.5f,  0.5f,  0.5f, 1.0f } };

		eiBox.min = { min.x, min.y, min.z };
		eiBox.max = { max.x, max.y, max.z };
	}

	bool Voxel::CheckCameraRayIntersection ( float & distance, Sides & hitSide ) const
	{
		glm::vec3 cameraPos { world->camera->GetPosition () };
		glm::vec3 cameraDir { world->camera->GetTargetDirection () };

		ei::Ray ray ( { cameraPos.x, cameraPos.y, cameraPos.z }, { cameraDir.x, cameraDir.y, cameraDir.z } );

		ei::HitSide eiHitSide;
		bool intersects { ei::intersects ( ray, eiBox, distance, eiHitSide ) };

		switch ( eiHitSide )
		{
		case ei::HitSide::X_NEG: hitSide = Sides::left;		break;
		case ei::HitSide::X_POS: hitSide = Sides::right;	break;
		case ei::HitSide::Y_NEG: hitSide = Sides::down;		break;
		case ei::HitSide::Y_POS: hitSide = Sides::up;		break;
		case ei::HitSide::Z_NEG: hitSide = Sides::back;		break;
		case ei::HitSide::Z_POS: hitSide = Sides::forward;	break;
		}

		return intersects;
	}

	std::vector <Voxel *> Voxel::GetNeighbours () const
	{
		std::vector <Voxel *> neighbours;
		neighbours.reserve ( 6 );

		for ( auto const & neighbour : this->neighbours )
			if ( neighbour )
				neighbours.push_back ( neighbour );
		
		return neighbours;
	}

	bool Voxel::CheckNeighbour ( Sides side ) const
	{
		return neighbours [ static_cast < int > ( side ) ] != nullptr;
	}

	bool Voxel::CheckVisible () const
	{
		return neighbours [ 0 ] || neighbours [ 1 ] || neighbours [ 2 ] || neighbours [ 3 ] || neighbours [ 4 ] || neighbours [ 5 ];
	}
	
	void Voxel::FindNeighbours ()
	{
		neighbours = {
			world->FindVoxel ( GetPosition () + GetDirectionVector ( Sides::up ) ),
			world->FindVoxel ( GetPosition () + GetDirectionVector ( Sides::down ) ),
			world->FindVoxel ( GetPosition () + GetDirectionVector ( Sides::left ) ),
			world->FindVoxel ( GetPosition () + GetDirectionVector ( Sides::right ) ),
			world->FindVoxel ( GetPosition () + GetDirectionVector ( Sides::forward ) ),
			world->FindVoxel ( GetPosition () + GetDirectionVector ( Sides::back ) ),
		};
	}
}
