#include "Cube.h"

void Cube::initVerts()
{
	std::cout << "Cube::initVerts()" << '\n';

	float VAO[] = {
		-0.5f, -0.5f, -0.5f,   0.1f, 0.1f, 0.1f,
		 0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f,

		-0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 1.0f
	};

	unsigned int VBO[] = {
		// �ո�
		4, 5, 6, 6, 7, 4,
		// �޸�
		0, 1, 2, 2, 3, 0,
		// ���� ��
		0, 3, 7, 7, 4, 0,
		// ������ ��
		1, 5, 6, 6, 2, 1,
		// �Ʒ��� ��
		0, 1, 5, 5, 4, 0,
		// ���� ��
		3, 2, 6, 6, 7, 3
	};

	isLine = false;
	shapeVertex = new Vertex(VAO, 8, VBO, 36);
}

void Cube::Update()
{
}

void Cube::Draw()
{
}

void Cube::openFace()
{
}
