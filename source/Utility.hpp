#pragma once

namespace vw
{
	GLuint CreateTextureFromFile ( std::string const & filePath );

	template < class Type >
	GLuint CreateBufferWithData ( std::vector <Type> const & data, GLenum target );


	// Implementation
	template < class Type >
	GLuint CreateBufferWithData ( std::vector <Type> const & data, GLenum target )
	{
		GLuint buffer;
		glGenBuffers ( 1, &buffer );
		glBindBuffer ( target, buffer );
		glBufferData ( target, data.size () * sizeof ( Type ), data.data (), GL_STATIC_DRAW );
		glBindBuffer ( target, 0 );
		return buffer;
	}
}