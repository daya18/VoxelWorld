#include "Core.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace vw
{
	std::unordered_map <Sides, glm::vec3> const directionVectors
	{
		{ Sides::up,		{  0,  1,  0 } },
		{ Sides::down,		{  0, -1,  0 } },
		{ Sides::left,		{ -1,  0,  0 } },
		{ Sides::right,		{  1,  0,  0 } },
		{ Sides::forward,	{  0,  0,  1 } },
		{ Sides::back,		{  0,  0, -1 } },
	};

	std::unordered_map <Sides, Sides> const oppositeSides
	{
		{ Sides::up,		Sides::down },
		{ Sides::down,		Sides::up },
		{ Sides::left,		Sides::right },
		{ Sides::right,		Sides::left },
		{ Sides::forward,	Sides::back },
		{ Sides::back,		Sides::forward },
	};

	std::array <Sides, 6> const sides { Sides::up, Sides::down, Sides::left, Sides::right, Sides::forward, Sides::back };

	std::unordered_map <Sides, std::string> const sideNames
	{
		{ Sides::up,		"Up" },
		{ Sides::down,		"Down" },
		{ Sides::left,		"Left" },
		{ Sides::right,		"Right" },
		{ Sides::forward,	"Forward" },
		{ Sides::back,		"Back" },
	};


	Sides GetOppositeSide ( Sides side )
	{
		switch ( side )
		{
		case Sides::up:			return Sides::down;
		case Sides::down:		return Sides::up;
		case Sides::left:		return Sides::right;
		case Sides::right:		return Sides::left;
		case Sides::forward:	return Sides::back;
		case Sides::back:		return Sides::forward;
		}
	}

	glm::vec3 GetDirectionVector ( Sides side )
	{
		switch ( side )
		{
		case Sides::up:			return { 0,  1,  0 };
		case Sides::down:		return { 0, -1,  0 };
		case Sides::left:		return { -1,  0,  0 };
		case Sides::right:		return { 1,  0,  0 };
		case Sides::forward:	return { 0,  0,  1 };
		case Sides::back:		return { 0,  0, -1 };
		}
	}

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
		glGenerateMipmap ( GL_TEXTURE_2D );
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
		glBindTexture ( GL_TEXTURE_2D, 0 );

		return texture;
	}

	GLuint CreateShader ( std::string const & filePath, GLenum type )
	{
		auto shader { glCreateShader ( type ) };

		std::string data { ( std::ostringstream {} << std::ifstream { filePath }.rdbuf () ).str () };

		GLchar const * shaderData { data.data () };
		GLint length { static_cast < GLint > ( data.size () ) };

		glShaderSource ( shader, 1, &shaderData, &length );
		glCompileShader ( shader );
		return shader;
	}

	GLuint CreateShaderProgram ( std::string const & vertexShaderFilePath, std::string const & fragmentShaderFilePath )
	{
		GLuint vertexShader { CreateShader ( vertexShaderFilePath, GL_VERTEX_SHADER ) };
		GLuint fragmentShader { CreateShader ( fragmentShaderFilePath, GL_FRAGMENT_SHADER ) };

		GLuint shaderProgram = glCreateProgram ();

		glAttachShader ( shaderProgram, vertexShader );
		glAttachShader ( shaderProgram, fragmentShader );
		glLinkProgram ( shaderProgram );

		glDeleteShader ( vertexShader );
		glDeleteShader ( fragmentShader );

		return shaderProgram;
	}
}