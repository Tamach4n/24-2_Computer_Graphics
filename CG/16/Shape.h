#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <random>
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
	Shape(const Shape& other);
	~Shape() { delete shapeVertex; }

	void clearBuffer();

	void initAxisVerts();
	void initSpiralVerts();
	void initCubeVerts();
	void initConeVerts();
	void initSphereVerts();

	void setActive(Shader* shader);
	void setState(bool st);
	void setMawari(bool rot);
	void setPos(float x, float y);
	void setRotation(float x, float y);
	void setDirection(float x, float y);
	void setMode(int m);
	void Reset();

	void Spiral(int i);
	void changePos(int i);
	void Orbit(int size);
	void changePos3d(int i);
	void Rotation(int size);

	void changeShape();

	bool getState() const { return rotation; }

	void Update();

	void Draw();

private:
	Vertex* shapeVertex;
	bool state;
	bool rotation;	//	1: ����, 0: ����
	bool isLine;
	int mode;

	void (Shape::* funcPtr)(int i);
	
	float xPos, yPos, zPos;
	float xDeg, yDeg, zDeg;

	float xDir, yDir, zDir;
	float xRot, yRot, zRot;	//	x�� �Ʒ���, y�� �ð� �ݴ� �������� ȸ��
	
	std::random_device rd;
};

