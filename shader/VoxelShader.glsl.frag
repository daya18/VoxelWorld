#version 460 core

layout ( location = 0 ) in vec2 i_texCoords;
layout ( location = 1 ) in vec3 i_positionModelSpace;
layout ( location = 2 ) in vec3 i_normal;
layout ( location = 3 ) in flat int i_currentVoxelTransformIndex;

layout ( location = 0 ) out vec4 o_color;

uniform sampler2D u_texture;
uniform int u_highligtedVoxelTransformIndex;

void main ()
{
	o_color = texture ( u_texture, i_texCoords );
	
	if ( u_highligtedVoxelTransformIndex != -1 )
	{
		if ( i_currentVoxelTransformIndex == u_highligtedVoxelTransformIndex )
			o_color.rgb += vec3 ( 0.4, 0.4, 0.4 );
	}
}