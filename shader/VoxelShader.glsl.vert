#version 460 core

layout ( location = 0 ) in vec3 position;

uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;

void main ()
{
	gl_Position = u_projectionMatrix * u_viewMatrix * vec4 ( position, 1.0f );
}