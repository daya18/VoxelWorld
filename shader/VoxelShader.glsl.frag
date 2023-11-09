#version 460 core

layout ( location = 0 ) in vec2 i_texCoords;

layout ( location = 0 ) out vec4 o_color;

uniform sampler2D u_texture;

void main ()
{
	o_color = texture ( u_texture, i_texCoords );
	//o_color = vec4 ( 1, 1, 1, 1 ); 
}