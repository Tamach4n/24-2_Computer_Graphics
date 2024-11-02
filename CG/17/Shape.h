#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <random>
#include "glew.h"
#include <glut.h>
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "Vertex.h"
#include "Shader.h"

class Shape
{
public:
	Shape();
	Shape(float x, float y, float z);
	~Shape() { delete shapeVertex; }

	virtual void clearBuffer();

	virtual void initVerts();
	virtual void initAxisVerts();

	virtual void setActive(Shader* shader);
	virtual void setRotateY();
	virtual void setPos(float x, float y, float z);
	virtual void setRotation(float x, float y, float z);
	virtual void setDirection(float x, float y, float z);
	virtual void Reset();

	virtual void rotation();
	virtual void openFace();

	virtual bool getState() const { return rotateY; }

	virtual void Update();

	virtual void Draw();

protected:
	Vertex* shapeVertex;
	bool rotateY;	//	Y�� ����
	bool isLine;
	
	float xPos, yPos, zPos;
	float xDeg, yDeg, zDeg;

	float xDir, yDir, zDir;
	float xRot, yRot, zRot;	//	x�� �Ʒ���, y�� �ð� �ݴ� �������� ȸ��
	
	std::random_device rd;
};

