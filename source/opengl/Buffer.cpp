#include "Buffer.hpp"

namespace vw::gl
{
	Buffer::Buffer ()
	{
		glGenBuffers ( 1, &buffer );
	}

	Buffer::Buffer ( void const * data, unsigned int size )
		: Buffer ()
	{
		SetData ( data, size );
	}

	Buffer::Buffer ( Buffer const & r )
	{
		glGenBuffers ( 1, &buffer );
		operator = ( r );
	}

	Buffer::Buffer ( Buffer && r ) noexcept
	{
		operator = ( std::move ( r ) );
	}

	Buffer::~Buffer ()
	{
		glDeleteBuffers ( 1, & buffer );
	}

	Buffer & Buffer::operator = ( Buffer const & r )
	{
		if ( size != 0 )
		{
			glBindBuffer ( GL_COPY_WRITE_BUFFER, buffer );
			glBufferData ( GL_COPY_WRITE_BUFFER, r.size, nullptr, GL_STATIC_DRAW );
			glBindBuffer ( GL_COPY_READ_BUFFER, r.buffer );
			glCopyBufferSubData ( r.buffer, buffer, 0, 0, r.size );
			glBindBuffer ( GL_COPY_READ_BUFFER, 0 );
			glBindBuffer ( GL_COPY_WRITE_BUFFER, 0 );
		}

		size = r.size;

		return *this;
	}

	Buffer & Buffer::operator = ( Buffer && r ) noexcept
	{
		buffer = std::exchange ( r.buffer, 0 );
		size = std::exchange ( r.size, 0 );

		return *this;
	}

	void Buffer::SetData ( void const * data, unsigned int size )
	{
		glBindBuffer ( GL_ARRAY_BUFFER, buffer );
		glBufferData ( GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW );
		glBindBuffer ( GL_ARRAY_BUFFER, 0 );

		this->size = size;
	}

	GLuint Buffer::GetBuffer () const
	{
		return buffer;
	}

	unsigned int Buffer::GetSize () const
	{
		return size;
	}
}