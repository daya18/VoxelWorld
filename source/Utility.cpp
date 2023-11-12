#include "Utility.hpp"

namespace vw
{
	GLuint CreateTextureFromFile ( std::string const & texturePath )
	{
		assert ( std::filesystem::exists ( texturePath ) );

		stbi_set_flip_vertically_on_load ( 1 );

		int width, height;
		auto data { stbi_load ( texturePath.data (), &width, &height, nullptr, STBI_rgb_alpha ) };

		GLuint texture;

		glGenTextures ( 1, &texture );
		glBindTexture ( GL_TEXTURE_2D, texture );
		glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
		glBindTexture ( GL_TEXTURE_2D, 0 );

		return texture;
	}
}
