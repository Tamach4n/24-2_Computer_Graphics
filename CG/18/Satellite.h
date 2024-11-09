#pragma once
#include "Shape.h"
#include "Planet.h"

class Satellite : public Shape
{
public:
	Satellite(const class Planet& p, float r, float degree);
	~Satellite() { delete shapeVertex; }

	void initOrbitVerts(const Position& center);

	void Update(const Planet& pla);

	void Draw(GLuint shaderProgram, const glm::mat4& pMat, const Position& pPos);

private:

};

