#version 460 core

layout ( location = 0 ) in vec2 i_position;
layout ( location = 1 ) in vec2 i_textureCoordinates;

layout ( location = 0 ) out vec2 o_textureCoordinates;

layout ( location = 0 ) uniform mat4 u_projectionMatrix;
layout ( location = 1 ) uniform vec2 u_position;
layout ( location = 2 ) uniform vec2 u_scale;

void main ()
{
	gl_Position = u_projectionMatrix * vec4 ( ( i_position * u_scale ) + u_position, 0, 1 );
	o_textureCoordinates = i_textureCoordinates;
}