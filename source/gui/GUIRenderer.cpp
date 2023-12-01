#include "GUIRenderer.hpp"

#include "../Application.hpp"
#include "../Window.hpp"

namespace vw
{
	GUIRenderer::GUIRenderer ( Application & application, Window & window )
	:
		application ( & application ),
		window ( & window ),
		shader ( "shader/GUIShader" ),
		vertexArray ( { { GL_FLOAT, 2 }, { GL_FLOAT, 2 } } ),

		vertexBuffer ( std::vector <GLfloat> {
		// Positions   // UVs
			 1, 0,		1, 1,
			 0, 0,		0, 1,
			 0, 1,		0, 0,
			 1, 1,		1, 0
		} ),

		indexBuffer ( std::vector <GLuint> { 0, 1, 2, 2, 3, 0 } )
	{
		UpdateProjection ( window.GetSize () );

		window.AddFramebufferResizeCallback ( [this] ( glm::vec2 const & size )
			{
				UpdateProjection ( size );
			}
		);
	}

	void GUIRenderer::RenderPanel ( Panel const & panel )
	{
		shader.Use ();

		shader.SetUniform ( "u_position", panel.position );
		shader.SetUniform ( "u_scale", panel.size );
		shader.SetUniform ( "u_projectionMatrix", projectionMatrix );
		shader.SetUniform ( "u_color", panel.color );
		
		auto texture { panel.texture != 0 ? panel.texture : application->GetWhiteTexture () };
		
		glActiveTexture ( GL_TEXTURE0 );
		glBindTexture ( GL_TEXTURE_2D, texture );
		shader.SetUniform ( "u_texture", 0 );
		
		vertexArray.Bind ();
		vertexArray.BindVertexBuffer ( vertexBuffer.GetBuffer () );
		vertexArray.BindIndexBuffer ( indexBuffer.GetBuffer () );

		glDrawElements ( GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr );
	}

	void GUIRenderer::UpdateProjection ( glm::vec2 viewportSize )
	{
		projectionMatrix = glm::ortho <float> (
			0.0f, viewportSize.x,
			viewportSize.y, 0.0f,
			-1.0f, 1.0f
		);
	}

	glm::vec2 GUIRenderer::GetWindowSize ()
	{
		return window->GetSize ();
	}

}