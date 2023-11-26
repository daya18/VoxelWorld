#pragma once

namespace vw::gl
{
	class VertexArray
	{
	public:
		struct Attribute
		{
			GLenum type;
			GLuint count;
		};

		VertexArray ();
		VertexArray ( std::vector <Attribute> const & );
		~VertexArray ();

		void SetAttributes ( std::vector <Attribute> const & );

		void BindVertexBuffer ( GLuint buffer );
		void BindIndexBuffer ( GLuint buffer );

		void Bind ();

	private:
		std::vector <Attribute> attributes;
		GLuint vertexSize;
		GLuint vertexArray;
	};
}