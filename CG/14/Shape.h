#pragma once
#
#include "gl/glew.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "Vertex.h"
#include "Shader.h"

class Shape
{
public:
	Shape();
	~Shape() { delete shapeVertex; }

	virtual void Init();

	virtual void setActive(Shader* shader);
	virtual void setRotation(float x, float y);
	virtual void setDirection(float x, float y);
	virtual void Reset();

	virtual void Update();

	virtual void Draw();

protected:
	Vertex* shapeVertex;
	float xPos, yPos;
	float xDeg, yDeg;

	float xDir, yDir;
	float xRot, yRot;	//	x는 아래로, y는 시계 반대 방향으로 회전
};

