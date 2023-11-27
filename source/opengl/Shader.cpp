#include "Shader.hpp"

namespace vw
{
	Shader::Shader ()
	{
	}

	Shader::Shader ( std::string const & filePath )
	{
		LoadFromFile ( filePath );
	}

	Shader::~Shader ()
	{
		glDeleteProgram ( program );
	}

	Shader::Shader ( Shader const & r )
	{
		operator = ( r );
	}

	Shader::Shader ( Shader && r ) noexcept
	{
		operator = ( std::move ( r ) );
	}

	Shader & Shader::operator = ( Shader const & r )
	{
		LoadFromFile ( r.fileBasePath );
		return *this;
	}

	Shader & Shader::operator = ( Shader && r ) noexcept
	{
		fileBasePath = std::exchange ( r.fileBasePath, {} );
		program = std::exchange ( r.program, 0 );
		return *this;
	}
	
	void Shader::LoadFromFile ( std::string const & fileBasePath )
	{
		this->fileBasePath = fileBasePath;

		static const std::unordered_map <GLuint, std::string> shaderTypeFileExtensions
		{
			{ GL_VERTEX_SHADER, ".glsl.vert" },
			{ GL_FRAGMENT_SHADER, ".glsl.frag" }
		};

		std::vector <GLuint> shaders;

		for ( auto const & [ type, extension ] : shaderTypeFileExtensions )
		{
			std::string filePath { fileBasePath + extension };

			if ( std::filesystem::exists ( filePath ) )
				shaders.push_back ( CreateShader ( filePath, type ) );
		}

		program = glCreateProgram ();

		for ( auto shader : shaders )
			glAttachShader ( program, shader );
		
		glLinkProgram ( program );

		for ( auto shader : shaders )
			glDetachShader ( program, shader );
		
		for ( auto shader : shaders )
			glDeleteShader ( shader );
	}

	void Shader::Use () const
	{
		glUseProgram ( program );
	}

	void Shader::SetUniform ( std::string const & name, glm::mat4 const & matrix )
	{
		glUniformMatrix4fv ( GetUniformLocation ( name ), 1, GL_FALSE, glm::value_ptr ( matrix ) );
	}
	
	void Shader::SetUniform ( std::string const & name, int value )
	{
		glUniform1i ( GetUniformLocation ( name ), value );
	}

	void Shader::SetUniform ( std::string const & name, float value )
	{
		glUniform1f ( GetUniformLocation ( name ), value );
	}

	void Shader::SetUniform ( std::string const & name, glm::vec2 const & value )
	{
		glUniform2f ( GetUniformLocation ( name ), value.x, value.y );
	}

	void Shader::SetUniform ( std::string const & name, glm::vec3 const & value )
	{
		glUniform3f ( GetUniformLocation ( name ), value.x, value.y, value.z );
	}

	void Shader::SetUniform ( std::string const & name, glm::vec4 const & value )
	{
		glUniform4f ( GetUniformLocation ( name ), value.x, value.y, value.z, value.w );
	}

	void Shader::BindUniformBuffer ( std::string const & name, GLuint buffer )
	{
		auto locationIt { uniformLocations.find ( name ) };

		if ( locationIt == uniformLocations.end () )
		{
			auto location { glGetUniformBlockIndex ( program, name.data () ) };
			glUniformBlockBinding ( program, location, location );
			assert ( location != -1 );
			locationIt = uniformLocations.insert ( { name, location } ).first;
		}
	}
	
	GLuint Shader::CreateShader ( std::string const & filePath, GLuint type )
	{
		auto shader { glCreateShader ( type ) };

		std::string data { ( std::ostringstream {} << std::ifstream { filePath }.rdbuf () ).str () };

		GLchar const * shaderData { data.data () };
		GLint length { static_cast < GLint > ( data.size () ) };

		glShaderSource ( shader, 1, &shaderData, &length );
		glCompileShader ( shader );
		return shader;
	}

	GLuint Shader::GetUniformLocation ( std::string const & name )
	{
		auto locationIt { uniformLocations.find ( name ) };

		if ( locationIt == uniformLocations.end () )
		{
			auto location { glGetUniformLocation ( program, name.data () ) };
			assert ( location != -1 );
			locationIt = uniformLocations.insert ( { name, location } ).first;
		}

		return locationIt->second;
	}
}