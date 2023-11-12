#version 460 core

layout ( location = 0 ) in vec3 i_position;
layout ( location = 1 ) in vec2 i_texCoords;

layout ( location = 0 ) out vec2 o_texCoords;

uniform mat4 u_modelMatrices [1000];
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;

uniform u_Transforms
{
	mat4 transforms [100000];
} u_transforms;

uniform u_TransformIndices
{
	vec4 transformIndices [100000];
} u_transformIndices;

void main ()
{
	int transformIndex = int ( u_transformIndices.transformIndices [ gl_InstanceID ].x );
	mat4 modelMatrix = u_transforms.transforms [ transformIndex ];
	gl_Position = u_projectionMatrix * u_viewMatrix * modelMatrix * vec4 ( i_position, 1.0f );
	o_texCoords = i_texCoords;
}