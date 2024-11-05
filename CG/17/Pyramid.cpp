#include "Pyramid.h"

void Pyramid::initVerts()
{
	std::cout << "Pyramid::initVerts()" << '\n';
}

void Pyramid::Update()
{
	if (rotateY)
		yDeg += yDir;

	if (animeSideFaces)
		;

	if (animeOnceFace)
		;
}

void Pyramid::Draw(GLuint shaderProgram)
{
}

void Pyramid::openFace()
{
}

void Pyramid::setAnimeMode(int mode)
{
	switch (mode) {
	case 1:
		animeSideFaces = !animeSideFaces;
		break;

	case 2:
		animeOnceFace = !animeOnceFace;
		break;

	default:
		break;
	}
}
