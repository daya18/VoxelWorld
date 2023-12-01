#version 460 core

layout ( location = 0 ) in vec2 i_textureCoordinates;

layout ( location = 0 ) out vec4 o_color;

layout ( location = 30 ) uniform vec4 u_color;
layout ( location = 31 ) uniform sampler2D u_texture;

void main ()
{
	o_color = u_color * texture ( u_texture, i_textureCoordinates );
}