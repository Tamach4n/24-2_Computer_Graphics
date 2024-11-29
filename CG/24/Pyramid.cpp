#include "Pyramid.h"

Pyramid::Pyramid()
{
    std::cout << "Pyramid()" << '\n';
}

void Pyramid::Init()
{
	float VAO[] = {
         0.0f,  0.5f,  0.0f,   1.0f, 0.0f, 0.0f,     0.0000,  1.0000,  0.0000,

        -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,    -0.6626, -0.3491, -0.6626,
         0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,     0.6626, -0.3491, -0.6626,
         0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,     0.6626, -0.3491,  0.6626,
        -0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,    -0.6626, -0.3491,  0.6626
    };

	unsigned int VBO[] = {
        0, 1, 2,
        0, 2, 3,
        0, 3, 4,
        0, 4, 1,

        1, 2, 3,
        1, 3, 4 
	};

	shapeVertex = new Vertex(VAO, 5, VBO, 18);
}
