#pragma once

namespace vw
{
	template < class Type >
	class VectorBuffer
	{
	public:
		VectorBuffer ();

		void PushBack ( std::vector <Type> const & );
		void Erase ( std::vector <unsigned int> const & indices );
		unsigned int GetElementCount () const;
		GLuint Get () const;

	private:
		std::vector <Type> data;
		GLuint buffer { 0 };
	};



	// Implementation
	template < class Type >
	VectorBuffer<Type>::VectorBuffer ()
	{
		glGenBuffers ( 1, &buffer );
	}

	template < class Type >
	void VectorBuffer<Type>::PushBack ( std::vector <Type> const & elements )
	{
		data.insert ( data.end (), elements.begin (), elements.end () );
		glBindBuffer ( GL_UNIFORM_BUFFER, buffer );
		glBufferData ( GL_UNIFORM_BUFFER, data.size () * sizeof ( Type ), data.data (), GL_STATIC_DRAW );
	}
	
	template < class Type >
	void VectorBuffer<Type>::Erase ( std::vector <unsigned int> const & indices )
	{
		for ( auto index : indices )
			data.erase ( data.begin () + index );

		glBindBuffer ( GL_UNIFORM_BUFFER, buffer );
		glBufferData ( GL_UNIFORM_BUFFER, data.size () * sizeof ( Type ), data.data (), GL_STATIC_DRAW );
	}

	template < class Type >
	unsigned int VectorBuffer<Type>::GetElementCount () const { return data.size (); }

	template < class Type > GLuint VectorBuffer<Type>::Get () const { return buffer; }
}