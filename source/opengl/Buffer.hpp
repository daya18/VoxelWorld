#pragma once

namespace vw::gl
{
	class Buffer
	{
	public:
		Buffer ();
		Buffer ( void const * data, unsigned int size );
		template <class Type> Buffer ( std::vector <Type> const & data );
		Buffer ( Buffer const & );
		Buffer ( Buffer && ) noexcept;
		~Buffer ();

		Buffer & operator = ( Buffer const & );
		Buffer & operator = ( Buffer && ) noexcept;

		void SetData ( void const * data, unsigned int size );
		template <class Type> void SetData ( std::vector <Type> const & data );

		GLuint GetBuffer () const;
		unsigned int GetSize () const;

	private:
		GLuint buffer		{ 0 };
		unsigned int size	{ 0 };
	};



	// Implementation
	template <class Type> Buffer::Buffer ( std::vector <Type> const & data )
		: Buffer ()
	{
		SetData ( data );
	}

	template <class Type> void Buffer::SetData ( std::vector <Type> const & data )
	{
		SetData ( data.data (), data.size () * sizeof ( Type ) );
	}
}