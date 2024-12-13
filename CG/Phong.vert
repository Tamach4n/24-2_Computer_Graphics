#version 330 core

layout (location = 0) in vec3 inPos;		// 응용 프로그램에서 받아온 도형 좌표값
layout (location = 1) in vec3 inNormal;		// 점의 법선벡터
layout (location = 2) in vec2 inTexCoord;

out vec2 fragTexCoord;
out vec3 fragNormal;
out vec3 fragPos;

uniform mat4 modelTransform;
uniform mat4 viewTransform;
uniform mat4 projTransform;

void main()
{
	vec4 pos = vec4(inPos, 1.f) * modelTransform;
	gl_Position = projTransform * viewTransform * pos;

	fragPos = pos.xyz;
	fragNormal = transpose(inverse(mat3(modelTransform))) * inNormal;
	fragTexCoord = inTexCoord;
}
