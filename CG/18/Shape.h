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

	virtual void clearBuffer();

	virtual void initVerts(float radius, const Position& color);

	virtual void setActive(Shader* shader);
	virtual void setRotateY();
	virtual void setRotateZ();
	virtual void setMoveX(int st);
	virtual void setMoveY(int st);
	virtual void setMoveZ(int st);
	virtual void setPos(float x, float y, float z);
	virtual void setRotation(float x, float y, float z);
	virtual void setDirection(float x, float y, float z);
	virtual void setMove(float x, float y, float z);
	virtual void Reset();

	virtual Position getPos() const { return pos; }
	virtual Position getDeg() const { return deg; }
	virtual Position getRot() const { return rot; }


	virtual void Update(const Position& center);

	virtual void Draw(GLuint shaderProgram, const Position& center);

protected:
	Vertex* shapeVertex;
	Vertex* orbitVertex;

	float shapeAngle;
	float orbitRadius;
	float orbitSpeed;

	bool rotateX;
	bool rotateY;
	bool rotateZ;
	int moveX;
	int moveY;
	int moveZ;
		
	Position pos;
	Position deg;

	Position rot;
	Position dir;
	
	std::random_device rd;

	virtual std::vector<glm::vec3> readOBJ(std::string fileName, const Position& color);
};

