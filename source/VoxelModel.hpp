//#pragma once
//
//#include "Core.hpp"
//#include "impex.hpp"
//
//namespace vw
//{
//	class VoxelModel
//	{
//	public:
//		VoxelModel ();
//
//		impex::Mesh const & GetFaceMesh ( Sides side ) const;
//		impex::Scene const & GetScene () const;
//		VoxelModel & Transform ( glm::mat4 const & );
//
//	private:
//		impex::Scene scene;
//		std::vector <impex::Mesh> orderedMeshes;
//	};
//
//
//	// Implementation
//	
//	inline impex::Scene const & VoxelModel::GetScene () const { return scene; }
//
//}