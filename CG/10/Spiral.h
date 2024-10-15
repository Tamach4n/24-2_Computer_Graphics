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

	Spiral(const Spiral& other);

	void update(int mode);
	void draw(Shader* shader, int mode) const;
	void setMode(int mode);

private:
	int curVerts;
};