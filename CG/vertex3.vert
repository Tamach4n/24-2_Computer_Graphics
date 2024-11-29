#version 330 core

layout (location = 0) in vec3 vPos;		// ���� ���α׷����� �޾ƿ� ���� ��ǥ��
layout (location = 1) in vec3 vColor;		// ���� ����
layout (location = 2) in vec3 vNormal;		// ���� ��������

out vec3 outColor;
out vec3 outFragPos;
out vec3 outNormal;

uniform mat4 modelTransform;
uniform mat4 viewTransform;
uniform mat4 projTransform;

void main()
{
	outColor = vColor;
	outFragPos = vec3(modelTransform * vec4(vPos, 1.f));
	outNormal = mat3(modelTransform) * vNormal;	// x, y, z�࿡ ���Ͽ� ���� ũ���� ����
	// outNormal = transpose(inverse(mat3(modelTransform))) * inNormal;		// x, y, z�࿡ ���Ͽ� �ٸ� ũ���� ����
	// normal�� �̵��� �ϸ� �ȵȴ�!		// x, y, z�࿡ ���Ͽ� �յ� ���/Ȯ���� �������..-> inverse == transpose

	gl_Position = projTransform * viewTransform * vec4(outFragPos, 1.f);
}
