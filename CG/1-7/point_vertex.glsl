#version 330 core

out vec3 outColor;		// fragment shader�� ���!�� �� �� �ְ� ���ش�!

uniform vec2 uPos;

void main()
{
	gl_Position = vec4(uPos, 0.f, 1.f);
	outColor = vec3(1.f, 0.f, 0.f);
}