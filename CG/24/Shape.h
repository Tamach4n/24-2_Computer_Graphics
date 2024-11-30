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
	virtual void setPosition(glm::vec3 p);
	virtual void setRotation(float x, float y, float z);
	virtual void setDirection(float x, float y, float z);
	virtual void setDegree(glm::vec3 degree) { deg = degree; }
	virtual void setScale(float scale) { mScale = scale; }
	virtual void setOrbit(bool orbit) { mOrbit = orbit; }
	virtual void setOrbitPos(const glm::vec3& oPos);
	virtual void setRadius(const float& r);
	virtual void Reset();

	virtual glm::vec3 getPosition() const { return pos; }
	virtual glm::vec3 getRotation() const { return deg; }

	virtual void Update();

	virtual void Draw(const Shader* shaderProgram);

protected:
	Vertex* shapeVertex;
	glm::vec3 pos;
	glm::vec3 deg;

	glm::vec3 dir;
	glm::vec3 rot;	//	x는 아래로, y는 시계 반대 방향으로 회전

	float mRadius;
	float mScale;
	bool mOrbit = false;
};

