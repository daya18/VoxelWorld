#pragma once

namespace vw
{
	class Shader
	{
	public:
		Shader ();
		Shader ( std::string const & filePath );
		~Shader ();
		Shader ( Shader const & );
		Shader ( Shader && ) noexcept;

		Shader & operator = ( Shader const & );
		Shader & operator = ( Shader && ) noexcept;

		void LoadFromFile ( std::string const & filePath );

		void Use () const;
		
		void SetUniform ( std::string const & name, glm::mat4 const & );
		void SetUniform ( std::string const & name, int );
		void SetUniform ( std::string const & name, float );
		void SetUniform ( std::string const & name, glm::vec2 const & );
		void SetUniform ( std::string const & name, glm::vec3 const & );
		void SetUniform ( std::string const & name, glm::vec4 const & );

		void BindUniformBuffer ( std::string const & name, GLuint buffer );

	private:
		GLuint CreateShader ( std::string const & filePath, GLuint type );
		GLuint GetUniformLocation ( std::string const & );

		std::string fileBasePath;
		GLuint program { 0 };
		std::unordered_map <std::string, GLuint> uniformLocations;
	};
}