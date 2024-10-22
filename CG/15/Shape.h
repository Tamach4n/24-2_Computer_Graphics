#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include "gl/glew.h"
#include <gl/glut.h>
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "Vertex.h"
#include "Shader.h"

class Shape
{
public:
	Shape();
	Shape(float x, float y);
	~Shape() { delete shapeVertex; }

	void clearBuffer();

	void InitCubeVerts();
	void InitConeVerts();
	void InitSphereVerts();

	void setActive(Shader* shader);
	void setRotation(float x, float y);
	void setDirection(float x, float y);
	void Reset();

	void Update();

	void Draw();

private:
	Vertex* shapeVertex;
	int rotation;	//	0: ����, 1: ����, 2: ����
	float xPos, yPos;
	float xDeg, yDeg;

	float xDir, yDir;
	float xRot, yRot;	//	x�� �Ʒ���, y�� �ð� �ݴ� �������� ȸ��
};

