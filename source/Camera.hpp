#pragma once

#include "SignalEmitter.hpp"

namespace vw
{
	class Window;

	class Camera : public SignalEmitter
	{
	public:
		Camera ( Window & );

		glm::mat4 const & GetViewMatrix () const;
		glm::mat4 const & GetProjectionMatrix () const;
		
		void Update ( float deltaTime );

	private:
		static float moveSensitivity;
		static float rotateSensitivity;

		void Rotate ( glm::vec3 normal, float angle );
		void Move ( glm::vec3 const & );
		void HandleKeyPress ( int key );
		void HandleKeyRelease ( int key );
		void UpdateViewMatrix ();
		void UpdateProjectionMatrix ();
		glm::vec3 GetRight () const;
		glm::vec3 GetForward () const;

		Window * window;

		glm::vec3 position;
		glm::vec3 lookDirection;

		glm::mat4 projectionMatrix;
		glm::mat4 viewMatrix;
	};

	// Implementation
	inline glm::mat4 const & Camera::GetViewMatrix () const { return viewMatrix; }
	inline glm::mat4 const & Camera::GetProjectionMatrix () const { return projectionMatrix; }
	inline glm::vec3 Camera::GetRight () const { return glm::normalize ( glm::cross ( lookDirection, glm::vec3 ( 0, 1, 0 ) ) ); }
	inline glm::vec3 Camera::GetForward () const { return glm::normalize ( lookDirection ); }
}