#pragma once

#include "../OpenGL/VertexArray.hpp" 
#include "../OpenGL/Buffer.hpp"
#include "../OpenGL/Shader.hpp"

#include "Widgets.hpp"

namespace vw
{
	class Application;
	class Window;

	class GUIRenderer
	{
	public:
		GUIRenderer ( Application &, Window & );

		void RenderPanel ( Panel const & );
		glm::vec2 GetWindowSize ();

	private:
		void UpdateProjection ( glm::vec2 viewportSize );

		Application * application;
		Window * window;
		Shader shader;
		gl::VertexArray vertexArray;
		gl::Buffer vertexBuffer;
		gl::Buffer indexBuffer;
		glm::mat4 projectionMatrix;
	};
}