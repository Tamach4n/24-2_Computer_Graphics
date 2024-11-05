#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vColor;

out vec3 outColor;		// fragment shader와 통신!을 할 수 있게 해준다!

//uniform vec3 uColor;
uniform mat4 modelTransform;	// C++ 프로그램(CPU)과 통신(값을 주고받을 수 있다) CPU프로그램과 전역변수 느낌으로 값을 주고받는다
uniform mat4 viewTransform;
uniform mat4 projTransform;

void main()
{
	gl_Position = projTransform * viewTransform * modelTransform * vec4(vPos, 1.f);
	outColor = vColor;
}
