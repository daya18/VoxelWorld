#include "VertexArray.hpp"

#include "Type.hpp"

namespace vw::gl
{
	VertexArray::VertexArray ()
	{
		glGenVertexArrays ( 1, &vertexArray );
	}

	VertexArray::VertexArray ( std::vector <Attribute> const & attributes )
		: VertexArray ()
	{

	}

	VertexArray::~VertexArray ()
	{
		glDeleteVertexArrays ( 1, &vertexArray );
	}
	
	void VertexArray::SetAttributes ( std::vector <Attribute> const & attributes )
	{
		Bind ();

		vertexSize = 0;

		GLuint attributeIndex { 0 };
		for ( auto const & attribute : attributes )
		{
			glEnableVertexAttribArray ( attributeIndex );
			glVertexAttribFormat ( attributeIndex, attribute.count, attribute.type, GL_FALSE, vertexSize );
			glVertexAttribBinding ( attributeIndex, 0 );
			++attributeIndex;
			vertexSize += GetTypeSize ( attribute.type ) * attribute.count;
		}
	}

	void VertexArray::BindVertexBuffer ( GLuint buffer )
	{
		Bind ();
		glBindVertexBuffer ( 0, buffer, 0, vertexSize );
	}
	
	void VertexArray::BindIndexBuffer ( GLuint buffer )
	{
		Bind ();
		glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, buffer );
	}

	void VertexArray::Bind ()
	{
		glBindVertexArray ( vertexArray );
	}
}