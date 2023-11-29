#include "Camera.hpp"

#include "Window.hpp"
#include "opengl/Shader.hpp"

namespace vw
{
	float Camera::moveSensitivity { 3.0f };
	float Camera::rotateSensitivity { 9.0f };

	Camera::Camera ( Window & window )
	:
		window ( & window ),
		position ( 0.0f, 0.0f, 5.0f ),
		lookDirection ( 0.0f, 0.0f, -1.0f )
	{
		UpdateViewMatrix ();
		UpdateProjectionMatrix ();

		window.AddFramebufferResizeCallback ( [this] ( glm::vec2 const & size )
			{
				UpdateViewMatrix ();
				UpdateProjectionMatrix ();
			}
		);
	}

	void Camera::Update ( float deltaTime )
	{
		// Handle Move
		if ( window->GetKey ( GLFW_KEY_D ) == GLFW_PRESS )
			Move ( GetRight () * moveSensitivity * deltaTime );
			
		if ( window->GetKey ( GLFW_KEY_A ) == GLFW_PRESS )
			Move ( -GetRight () * moveSensitivity * deltaTime );
		
		if ( window->GetKey ( GLFW_KEY_W ) == GLFW_PRESS )
			Move ( glm::cross ( glm::vec3 { 0, 1, 0 }, GetRight () ) * moveSensitivity * deltaTime );

		if ( window->GetKey ( GLFW_KEY_S ) == GLFW_PRESS )
			Move ( -glm::cross ( glm::vec3 { 0, 1, 0 }, GetRight () ) * moveSensitivity * deltaTime );
		
		if ( window->GetKey ( GLFW_KEY_SPACE ) == GLFW_PRESS )
			Move ( glm::vec3 { 0.0f, 1.0f, 0.0f } * moveSensitivity * deltaTime );

		if ( window->GetKey ( GLFW_KEY_LEFT_SHIFT ) == GLFW_PRESS )
			Move ( glm::vec3 { 0.0f, -1.0f, 0.0f } * moveSensitivity * deltaTime );

		// Handle rotation
		Rotate ( { 0.0f, 1.0f, 0.0f }, -window->GetMouseDelta ().x * rotateSensitivity * deltaTime );
		Rotate ( GetRight (), -window->GetMouseDelta ().y * rotateSensitivity * deltaTime );
		
		UpdateViewMatrix ();
	}

	void Camera::Bind ( Shader & shader )
	{
		shader.SetUniform ( "u_viewMatrix", viewMatrix );
		shader.SetUniform ( "u_projectionMatrix", projectionMatrix );
	}

	void Camera::Rotate ( glm::vec3 normal, float angle )
	{
		lookDirection = glm::rotate ( lookDirection, glm::radians ( angle ), normal );
		//UpdateViewMatrix ();
	}

	void Camera::Move ( glm::vec3 const & delta )
	{
		position += delta;
		//UpdateViewMatrix ();
	}

	void Camera::UpdateViewMatrix ()
	{
		viewMatrix = glm::lookAt ( position, position + lookDirection, { 0.0f, 1.0f, 0.0f } );
	}

	void Camera::UpdateProjectionMatrix ()
	{
		glm::vec2 windowSize { window->GetSize () };
		projectionMatrix = glm::perspective ( 45.0f, windowSize.x / windowSize.y, 0.001f, 100.0f );
	}
}