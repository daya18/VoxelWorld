#version 460 core

layout ( location = 0 ) in vec3 i_position;
layout ( location = 1 ) in vec2 i_texCoords;
layout ( location = 2 ) in vec3 i_normal;

layout ( location = 0 ) out vec2 o_texCoords;
layout ( location = 1 ) out vec3 o_positionModelSpace;
layout ( location = 2 ) out vec3 o_normal;

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;

void main ()
{
	gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * vec4 ( i_position, 1.0f );
	o_texCoords = i_texCoords;
	o_positionModelSpace = i_position;
	o_normal = i_normal;
}