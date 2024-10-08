#pragma once
#include <gl/glew.h> // 필요한 헤더파일 include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <string>
#include <list>

struct Rect {
	GLfloat x1, y1, x2, y2;
};

struct Triangle {
	std::pair<float, float> coord;
	GLclampf r, g, b;
};

class Quadrant {
public:
	void draw(int mode, GLuint pos, GLuint color);

	bool isClicked(const float& mx, const float& my);

	int getListSize();

	void insert(float x, float y);

private:
	//Rect rect;
	std::list<Triangle> triangle;
};

