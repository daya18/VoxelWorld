#version 460 core

layout ( location = 0 ) in vec3 i_position;
layout ( location = 1 ) in vec2 i_texCoords;

layout ( location = 0 ) out vec2 o_texCoords;

uniform mat4 u_modelMatrices [1000];
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;

void main ()
{
	mat4 modelMatrix = u_modelMatrices [ gl_InstanceID ];
	gl_Position = u_projectionMatrix * u_viewMatrix * modelMatrix * vec4 ( i_position, 1.0f );
	o_texCoords = i_texCoords;
}