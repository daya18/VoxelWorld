#include "Camera.hpp"

#include "Window.hpp"

namespace vw
{
	float Camera::moveSensitivity { 0.01f };

	Camera::Camera ( Window & window )
	:
		window ( & window ),
		position ( 0.0f, 0.0f, -10.0f ),
		lookDirection ( 0.0f, 0.0f, 1.0f )
	{
		UpdateViewMatrix ();
		UpdateProjectionMatrix ();
	}

	void Camera::Update ( float deltaTime )
	{
		if ( window->GetKey ( GLFW_KEY_D ) == GLFW_PRESS )
			Move ( GetRight () * moveSensitivity );
			
		if ( window->GetKey ( GLFW_KEY_A ) == GLFW_PRESS )
			Move ( -GetRight () * moveSensitivity );
		
		if ( window->GetKey ( GLFW_KEY_W ) == GLFW_PRESS )
			Move ( GetForward () * moveSensitivity );

		if ( window->GetKey ( GLFW_KEY_S ) == GLFW_PRESS )
			Move ( -GetForward () * moveSensitivity );
		
		if ( window->GetKey ( GLFW_KEY_SPACE ) == GLFW_PRESS )
			Move ( glm::vec3 { 0.0f, 1.0f, 0.0f } * moveSensitivity );

		if ( window->GetKey ( GLFW_KEY_LEFT_SHIFT ) == GLFW_PRESS )
			Move ( glm::vec3 { 0.0f, -1.0f, 0.0f } * moveSensitivity );

	}

	void Camera::Move ( glm::vec3 const & delta )
	{
		position += delta;
		UpdateViewMatrix ();
	}

	void Camera::UpdateViewMatrix ()
	{
		viewMatrix = glm::lookAt ( position, position + lookDirection, { 0.0f, 1.0f, 0.0f } );
		EmitSignal ( "ViewUpdate" );
	}

	void Camera::UpdateProjectionMatrix ()
	{
		glm::vec2 windowSize { window->GetSize () };
		projectionMatrix = glm::perspective ( 45.0f, windowSize.x / windowSize.y, 0.001f, 100.0f );
		EmitSignal ( "ProjectionUpdate" );
	}
}