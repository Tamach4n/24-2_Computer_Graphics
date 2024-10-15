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

	void update(int size);
	void draw(Shader* shader, int mode) const;

private:
	float xPos, yPos;
	int curr;

	std::vector<std::pair<float, float>> pointVec;
	std::random_device rd;
};