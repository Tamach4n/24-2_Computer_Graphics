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

enum Direction {
	Dir_None, Dir_Left, Dir_Right, Dir_Front, Dir_Back
};

class Shape
{
public:
	Shape();
	Shape(float r, float degree);
	~Shape();

	virtual void clearBuffer();

	virtual void init();
	virtual void initBuffer();

	virtual void setActive(Shader* shader);
	//virtual void setPos(float x, float y, float z);
	//virtual void setRotation(float x, float y, float z);
	//virtual void setDirection(float x, float y, float z);
	//virtual void setMove(float x, float y, float z);

	virtual void Reset();

	virtual void Update();

	virtual void Draw(GLuint shaderProgram);

protected:
	Vertex* shapeVertex;
	
	std::random_device rd;
};

class Robot : public Shape {
public:
	Robot();
	~Robot() { delete shapeVertex; }

	void init() override;
	void initBuffer() override;

	void setJump() { isJumping = true; }
	void setGroundPos(float y);
	void setDir(Direction d);
	void adjSpeed(int i);

	glm::vec3 getPos() const { return pos; }

	void Update() override;

	void Draw(GLuint shaderProgram) override;
	void DrawParts(GLuint shaderProgram);

private:
	int armRotateLevel;	//	0: idle, 1: 앞으로 최대, 2: 뒤 최대
	float dArmAngle;
	float armAngle;

	bool isJumping;
	bool jumpPeak;
	float jumpPos;	//	JumpForce?

	glm::vec3 pos;
	float groundYPos;
	//glm::vec3 deg;	//	로봇이 바라볼 각도

	float Angle;
	//glm::vec3 rot;
	glm::vec3 dir;	//	이동 단위
	float speed;
};

class Cbstacle : public Shape {
public:
	Cbstacle();
	~Cbstacle() { delete shapeVertex; }

	void init() override;
	void initBuffer() override;

	void setState(bool st);

	glm::vec3 getPos() const { return pos; }
	bool getState() const { return isSteppedOn; }

	void Update() override;

	void Draw(GLuint shaderProgram) override;
	void DrawParts(GLuint shaderProgram);

private:
	bool isSteppedOn;
	float fumi;	//	밟힌 정도, footPressure
	glm::vec3 pos;
};

class Butai : public Shape {
public:
	Butai();
	~Butai() { delete shapeVertex; }

	void init() override;
	void initBuffer() override;

	void setOpen();

	void Update() override;

	void Draw(GLuint shaderProgram) override;

private:
	bool isOpening;
	bool isOpened;
	float doorPos;
};