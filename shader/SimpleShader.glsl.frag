#version 460 core

layout ( location = 0 ) in vec2 i_texCoords;
layout ( location = 1 ) in vec3 i_positionModelSpace;
layout ( location = 2 ) in vec3 i_normal;

layout ( location = 0 ) out vec4 o_color;

layout ( location = 33 ) uniform vec4 u_color;

void main ()
{
	o_color = u_color;
}