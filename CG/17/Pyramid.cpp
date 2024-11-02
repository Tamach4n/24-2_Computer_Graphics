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

void Pyramid::Draw()
{
}

void Pyramid::openFace()
{
}
