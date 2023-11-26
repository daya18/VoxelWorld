#pragma once

namespace vw
{
	template < class Type >
	class VectorBuffer
	{
	public:
		VectorBuffer ();
		~VectorBuffer ();
		VectorBuffer ( VectorBuffer const & ) = delete;;
		VectorBuffer ( VectorBuffer && ) = delete;;

		VectorBuffer & operator = ( VectorBuffer const & ) = delete;;
		VectorBuffer & operator = ( VectorBuffer && ) = delete;;

		void PushBack ( std::vector <Type> const & newElements );
		void Erase ( std::vector <unsigned int> const & indices );
		void Update ( std::vector <std::pair <unsigned int, Type>> const & );
		void Clear ();

		GLuint GetBuffer () const;
		unsigned int GetElementCount ();

	private:
		GLuint buffer { 0 };
		unsigned int elementCount { 0 };
	};



	// Implementation
	template < class Type >
	VectorBuffer<Type>::VectorBuffer ()
	{
	}

	template < class Type >
	VectorBuffer<Type>::~VectorBuffer ()
	{
		glDeleteBuffers ( 1, &buffer );
	}

	//template < class Type >
	//VectorBuffer<Type>::VectorBuffer ( VectorBuffer const & r )
	//{
	//	operator = ( r );
	//}

	//template < class Type >
	//VectorBuffer<Type>::VectorBuffer ( VectorBuffer && r )
	//{
	//	operator = ( std::move ( r ) );
	//}

	//template < class Type >
	//VectorBuffer<Type> & VectorBuffer<Type>::operator = ( VectorBuffer const & )
	//{

	//}

	//template < class Type >
	//VectorBuffer<Type> & VectorBuffer<Type>::operator = ( VectorBuffer && )
	//{

	//}
	
	template < class Type >
	void VectorBuffer<Type>::Clear ()
	{
		glDeleteBuffers ( 1, &buffer );
		buffer = 0;
		elementCount = 0;
	}

	template < class Type >
	void VectorBuffer<Type>::PushBack ( std::vector <Type> const & newElements )
	{
		GLuint newBuffer;
		glGenBuffers ( 1, &newBuffer );
		glBindBuffer ( GL_COPY_WRITE_BUFFER, newBuffer );
		glBufferData ( GL_COPY_WRITE_BUFFER, ( elementCount + newElements.size () ) * sizeof ( Type ), nullptr, GL_STATIC_DRAW );

		if ( elementCount > 0 )
		{
			glBindBuffer ( GL_COPY_READ_BUFFER, buffer );
			glCopyBufferSubData ( GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, elementCount * sizeof ( Type ) );
		}

		glBufferSubData (
			GL_COPY_WRITE_BUFFER,
			elementCount * sizeof ( Type ),
			newElements.size () * sizeof ( Type ),
			newElements.data ()
		);

		glDeleteBuffers ( 1, &buffer );
		buffer = newBuffer;
		elementCount += newElements.size ();
	}

	template < class Type >
	void VectorBuffer<Type>::Erase ( std::vector <unsigned int> const & indices )
	{
		for ( auto const & targetIndex : indices )
		{
			if ( elementCount == 1 )
			{
				elementCount = 0;
				return;
			}

			GLuint newBuffer;
			glGenBuffers ( 1, &newBuffer );
			glBindBuffer ( GL_COPY_WRITE_BUFFER, newBuffer );
			glBufferData ( GL_COPY_WRITE_BUFFER, ( elementCount - 1 ) * sizeof ( Type ), nullptr, GL_STATIC_DRAW );

			glBindBuffer ( GL_COPY_READ_BUFFER, buffer );

			glCopyBufferSubData ( GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER,
				0, 0, targetIndex * sizeof ( Type ) );

			glCopyBufferSubData ( GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER,
				( targetIndex + 1 ) * sizeof ( Type ),
				targetIndex * sizeof ( Type ),
				( elementCount - 1 - targetIndex ) * sizeof ( Type )
			);

			glDeleteBuffers ( 1, &buffer );
			buffer = newBuffer;
			--elementCount;
		}
	}
	
	template < class Type >
	void VectorBuffer<Type>::Update ( std::vector <std::pair <unsigned int, Type>> const & )
	{
	}

	template < class Type >
	GLuint VectorBuffer<Type>::GetBuffer () const
	{
		return buffer;
	}

	template < class Type >
	unsigned int VectorBuffer<Type>::GetElementCount ()
	{
		return elementCount;
	}

}