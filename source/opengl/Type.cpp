#include "Type.hpp"

namespace vw::gl
{
	GLuint GetTypeSize ( GLenum type )
	{
		switch ( type )
		{
		case GL_FLOAT: return sizeof ( GLfloat );
		case GL_INT: return sizeof ( GLint );
		case GL_UNSIGNED_INT: return sizeof ( GLuint );
		case GL_UNSIGNED_BYTE: return sizeof ( GLubyte );
		default: throw std::runtime_error { "Type size not defined" };
		}
	}
}