#version 330 core

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec3 inColor;

out vec3 outColor;

uniform vec2 uPos;
uniform vec3 uColor;

void main()
{
	gl_Position = vec4(inPosition + uPos, 0.f, 1.f);
	outColor = uColor;
	//outColor = inColor;
}