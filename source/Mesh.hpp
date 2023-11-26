#pragma once

#include "opengl/VertexArray.hpp"
#include "opengl/Buffer.hpp"

namespace vw
{
	class Mesh
	{
	public:
		Mesh ();
		Mesh ( std::string const & sceneFilePath, glm::mat4 const & transform, std::string const & MeshName = {} );
		
		void Bind ();

		GLuint GetIndexCount ();

	private:
		gl::VertexArray vertexArray;
		gl::Buffer vertexBuffer;
		gl::Buffer indexBuffer;
		GLuint indexCount;
	};
}