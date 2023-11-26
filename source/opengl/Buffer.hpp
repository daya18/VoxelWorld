#pragma once

namespace vw::gl
{
	class Buffer
	{
	public:
		Buffer ();
		Buffer ( void const * data, unsigned int size );
		Buffer ( Buffer const & );
		Buffer ( Buffer && ) noexcept;
		~Buffer ();

		Buffer & operator = ( Buffer const & );
		Buffer & operator = ( Buffer && ) noexcept;

		void SetData ( void const * data, unsigned int size );

		GLuint GetBuffer () const;
		unsigned int GetSize () const;

	private:
		GLuint buffer		{ 0 };
		unsigned int size	{ 0 };
	};
}