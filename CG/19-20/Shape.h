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

struct Position {
	float x, y, z;

	Position(float x = 0.f, float y = 0.f, float z = 0.f)
		:x(x), y(y), z(z) {}

	Position& operator+=(const Position& other) {
		this->x = this->x + other.x;
		this->y = this->y + other.y;
		this->z = this->z + other.z;
		return *this;
	}
};

class Shape
{
public:
	Shape();
	Shape(float r, float degree);
	Shape(const Shape& other);
	Shape& operator=(const Shape& other);
	~Shape();

	void clearBuffer();

	void init();
	void initBuffer();

	void setActive(Shader* shader);
	void setPos(float x, float y, float z);
	void setRotation(float x, float y, float z);
	void setDirection(float x, float y, float z);
	void setMove(float x, float y, float z);

	void setMoveX(int i);
	void setRotateTop(int i);
	void setRotateBarrel(int i);
	void setMoveBarrel(int i);
	void setRotateArm(int i);
	void Reset();

	Position getPos() const { return pos; }
	Position getDeg() const { return deg; }
	Position getRot() const { return rot; }

	void Update();

	void Draw(GLuint shaderProgram);

protected:
	Vertex* shapeVertex;

	float movePos;
	bool isMovePosiX;
	bool isMoveNegaX;

	float topRotateAngle;
	bool isTopRotatePosiY;
	bool isTopRotateNegaY;

	float barrelRotateAngle;
	bool isBarrelRotatePosiY;
	bool isBarrelRotateNegaY;

	float barrelMovePos;
	bool isBarrelmovePosiX;
	bool isBarrelmoveNegaX;

	float armRotateAngle;
	bool isArmRotatePosiZ;
	bool isArmRotateNegaZ;

	Position pos;
	Position deg;

	Position rot;
	Position dir;
	
	std::random_device rd;
};

