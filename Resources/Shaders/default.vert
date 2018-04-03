#version 450 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

uniform mat4 viewPrsp;
uniform mat4 model;

out VS_OUT {
	vec3 pos;
	vec3 normal;
	vec2 uv;
} vert;

void main()
{
	vert.pos = pos;
	vert.normal = normal;
	vert.uv = uv;
	
	gl_Position = viewPrsp * model * vec4(vert.pos, 1.0f);
}