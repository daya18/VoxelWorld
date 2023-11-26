#include "Mesh.hpp"

#include "OBJ_Loader.h"

namespace vw
{
	Mesh::Mesh ()
	{
	}

	Mesh::Mesh ( std::string const & sceneFilePath, glm::mat4 const & transform, std::string const & meshName )
	{

		objl::Loader loader;
		loader.LoadFile ( sceneFilePath );

		int meshIndex { 0 };

		if ( ! meshName.empty () )
		{
			int index { 0 };

			for ( auto const & mesh : loader.LoadedMeshes )
			{
				if ( mesh.MeshName == meshName )
				{
					meshIndex = index;
					break;
				}

				++index;
			}
		}
		
		vertexArray.SetAttributes ( { { GL_FLOAT, 3 }, { GL_FLOAT, 2 }, { GL_FLOAT, 3 } } );


		std::vector <GLfloat> vertexBufferData;
		
		for ( auto const & vertex : loader.LoadedMeshes [ meshIndex ].Vertices )
		{
			glm::vec3 position { vertex.Position.X, vertex.Position.Y, vertex.Position.Z };
			position = transform * glm::vec4 ( position, 1.0f );

			auto components = {
				position.x, position.y, position.z,
				vertex.TextureCoordinate.X, vertex.TextureCoordinate.Y,
				vertex.Normal.X, vertex.Normal.Y, vertex.Normal.Z
			};

			vertexBufferData.insert ( vertexBufferData.end (), components.begin (), components.end () );
		}


		std::vector <GLuint> indexBufferData;

		for ( auto const & index : loader.LoadedMeshes [ meshIndex ].Indices )
			indexBufferData.push_back ( index );

		vertexBuffer.SetData ( vertexBufferData.data (), vertexBufferData.size () * sizeof ( GLfloat ) );
		indexBuffer.SetData ( indexBufferData.data (), indexBufferData.size () * sizeof ( GLuint ) );

		vertexArray.BindVertexBuffer ( vertexBuffer.GetBuffer () );
		vertexArray.BindIndexBuffer ( indexBuffer.GetBuffer () );

		indexCount = loader.LoadedMeshes [ meshIndex ].Indices.size ();
	}

	void Mesh::Bind ()
	{
		vertexArray.Bind ();
	}

	GLuint Mesh::GetIndexCount ()
	{
		return indexCount;
	}

}