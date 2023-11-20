#include "impex.hpp"

#include "OBJ_Loader.h"

namespace vw::impex
{
	Scene LoadSceneFromFile ( std::string const & filePath )
	{
		Scene scene;

		objl::Loader loader;
		loader.LoadFile ( filePath );

		scene.meshes.reserve ( loader.LoadedMeshes.size () );

		for ( auto const & objlMesh : loader.LoadedMeshes )
		{
			Mesh mesh;
			
			mesh.vertices.reserve ( objlMesh.Vertices.size () );
			mesh.indices.reserve ( objlMesh.Indices.size () );

			for ( auto const & objlVertex : objlMesh.Vertices )
			{
				mesh.vertices.push_back ( {
					{ objlVertex.Position.X,objlVertex.Position.Y, objlVertex.Position.Z },
					{ objlVertex.TextureCoordinate.X,objlVertex.TextureCoordinate.Y },
					{ objlVertex.Normal.X,objlVertex.Normal.Y, objlVertex.Normal.Z },
				} );
			}

			for ( auto index : objlMesh.Indices )
				mesh.indices.push_back ( index );

			scene.meshes.insert ( { objlMesh.MeshName, mesh } );
		}

		scene.vertices.reserve ( loader.LoadedVertices.size () );
		scene.indices.reserve ( loader.LoadedIndices.size () );

		for ( auto const & vertex : loader.LoadedVertices )
		{
			scene.vertices.push_back ( {
				{ vertex.Position.X,vertex.Position.Y, vertex.Position.Z },
				{ vertex.TextureCoordinate.X,vertex.TextureCoordinate.Y },
				{ vertex.Normal.X,vertex.Normal.Y, vertex.Normal.Z },
			} );
		}

		for ( auto index : loader.LoadedIndices )
			scene.indices.push_back ( index );

		return scene;
	}
	
	Scene TransformScene ( Scene const & scene, glm::mat4 const & transformMatrix )
	{
		Scene outScene { scene };

		for ( auto & [ meshName, mesh ] : outScene.meshes )
			for ( auto & vertex : mesh.vertices )
				vertex.position = transformMatrix * glm::vec4 ( vertex.position, 1.0f );

		for ( auto & vertex : outScene.vertices )
			vertex.position = transformMatrix * glm::vec4 ( vertex.position, 1.0f );

		return outScene;
	}

	std::vector <GLfloat> InterleaveVertices ( std::vector <Vertex> const & vertices )
	{
		std::vector <float> outVertices;
		outVertices.reserve ( vertices.size () * ( 3 + 2 + 3 ) );

		for ( auto const & vertex : vertices )
		{
			auto outVertex = { 
				vertex.position.x, vertex.position.y, vertex.position.z,
				vertex.textureCoordinates.x, vertex.textureCoordinates.y
			};

			outVertices.insert ( outVertices.end (), outVertex.begin (), outVertex.end () );
		}

		return outVertices;
	}
}