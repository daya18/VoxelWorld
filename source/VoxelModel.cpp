//#include "VoxelModel.hpp"
//
//namespace vw
//{
//	VoxelModel::VoxelModel ()
//		: scene ( impex::LoadSceneFromFile ( "model/Voxel.obj" ) )
//	{
//	}
//	
//	impex::Mesh const & VoxelModel::GetFaceMesh ( Sides side ) const
//	{
//		switch ( side )
//		{
//		case Sides::up:			return orderedMeshes [0];
//		case Sides::down:		return orderedMeshes [1];
//		case Sides::left:		return orderedMeshes [2];
//		case Sides::right:		return orderedMeshes [3];
//		case Sides::forward:	return orderedMeshes [4];
//		case Sides::back:		return orderedMeshes [5];
//		}
//
//	}
//
//	VoxelModel & VoxelModel::Transform ( glm::mat4 const & transformMatrix )
//	{
//		scene = impex::TransformScene ( scene, transformMatrix );
//
//		auto orderedMeshNames = {
//			"Top_Face",
//			"Bottom_Face",
//			"Left_Face",
//			"Right_Face",
//			"Front_Face",
//			"Back_Face"
//		};
//
//		for ( auto const & orderedMeshName : orderedMeshNames )
//			orderedMeshes.push_back ( scene.meshes.at ( orderedMeshName ) );
//		
//		return *this;
//	}
//}