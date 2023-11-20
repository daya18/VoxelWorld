#pragma once

namespace vw
{
	GLuint CreateTextureFromFile ( std::string const & filePath );

	template < class Type >
	GLuint CreateBufferWithData ( std::vector <Type> const & data, GLenum target );

	template <class LType, class RType, class TransformFn >
	void Transform ( std::vector <LType> const &, std::vector <RType> &, TransformFn );

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

	template <class LType, class RType, class TransformFn >
	void Transform ( std::vector <LType> const & lvec, std::vector <RType> &rvec, TransformFn fn )
	{
		rvec.clear ();
		rvec.reserve ( lvec.size () );
		
		for ( auto const & lelem : lvec )
			rvec.push_back ( fn ( lelem ) );
	}
}