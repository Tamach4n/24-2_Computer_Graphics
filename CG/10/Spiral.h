#pragma once
#include <gl/glew.h> // 필요한 헤더파일 include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <vector>
#include <random>
#include "Shader.h"

class Spiral
{
public:
	Spiral();
	Spiral(float x, float y);
	Spiral(const Spiral& other);

	bool update(int mode);
	void draw(Shader* shader, int mode) const;
	void setDir();

private:
	float xPos, yPos;
	float angle;
	float radius;
	float angleSpeed;
	float radiusGrowth;
	bool step;
	int dir;

	std::vector<std::pair<float, float>> pointVec;
	std::random_device rd;
};