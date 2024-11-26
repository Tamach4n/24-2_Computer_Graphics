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

	virtual void Reset();

	virtual void Update();

	virtual void Draw(const Shader* shaderProgram);

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

	void setJump(bool st) { isJumping = st; }
	void setGroundPos(float y);
	void setDir(Direction d);
	void adjSpeed(int i);
	void steppingOn();
	void setCanMove(bool st) { canMove = st; }
	void setStepOn(bool st) { isSteppingOn = st; }
	bool checkCollision(const class Obstacle* obs);
	bool checkCollision(const class Butai* butai);

	glm::vec4 getHitbox() const { return glm::vec4(pos.x - 0.1f, pos.z - 0.1f, pos.x + 0.1f, pos.z + 0.1f); }
	glm::vec3 getPos() const { return pos; }

	void Update() override;

	void Draw(const Shader* shaderProgram) override;
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
	bool canMove;
	bool isSteppingOn;
	bool isMoving;	//	이럴거면 state Enum 만들지
};

class Obstacle : public Shape {
public:
	Obstacle();
	~Obstacle() { delete shapeVertex; }

	void init() override;
	void initBuffer() override;

	void setState(bool st);

	glm::vec4 getHitbox() const { return glm::vec4(pos.x - 0.2f, pos.z - 0.2f, pos.x + 0.2f, pos.z + 0.2f); }
	glm::vec3 getPos() const { return pos; }
	bool getState() const { return isSteppedOn; }

	void Update() override;

	void Draw(const Shader* shaderProgram) override;

private:
	bool isSteppedOn;
	glm::vec3 pos;
};

class Butai : public Shape {
public:
	Butai();
	~Butai() { delete shapeVertex; }

	void init() override;
	void initBuffer() override;

	void setOpen();

	glm::vec4 getArea() const { return glm::vec4(-1.f, -1.0, 1.f, 1.f); }

	void Update() override;

	void Draw(const Shader* shaderProgram) override;

private:
	bool isOpening;
	bool isOpened;
	float doorPos;
};