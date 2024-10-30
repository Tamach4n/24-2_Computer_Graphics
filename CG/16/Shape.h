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
	void setPos(float x, float y, float z);
	void setRotation(float x, float y, float z);
	void setDirection(float x, float y, float z);
	void setMode(int m);
	void setSpeed(int i);
	void setSize(float s);
	void Reset();

	void Spiral(int i);
	void changePos(int i);
	void Orbit(int size);
	void changePos3d(int i);
	void Rotation(int i);

	void changeShape();

	bool getState() const { return rotation; }

	void Update(int i);

	void Draw();

private:
	Vertex* shapeVertex;
	bool state;
	bool rotation;	//	1: 자전, 0: 공전
	bool isLine;
	bool isLines;
	bool moving;
	int mode;
	int countMove;
	float speed;

	float size;
	float dS;

	void (Shape::* funcPtr)(int i);
	
	float xPos, yPos, zPos;
	float xDeg, yDeg, zDeg;

	float xDir, yDir, zDir;
	float xRot, yRot, zRot;	//	x는 아래로, y는 시계 반대 방향으로 회전
	
	std::random_device rd;
};

