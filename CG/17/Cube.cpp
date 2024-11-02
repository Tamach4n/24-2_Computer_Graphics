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
		// 앞면
		4, 5, 6, 6, 7, 4,
		// 뒷면
		0, 1, 2, 2, 3, 0,
		// 왼쪽 면
		0, 3, 7, 7, 4, 0,
		// 오른쪽 면
		1, 5, 6, 6, 2, 1,
		// 아래쪽 면
		0, 1, 5, 5, 4, 0,
		// 위쪽 면
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
