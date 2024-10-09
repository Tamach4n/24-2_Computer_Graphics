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
	Quadrant();

	Quadrant(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);

	void draw(int mode, GLuint pos, GLuint color);

	void clearShapes();

	bool isClicked(const float& mx, const float& my) const;

	int getListSize();

	void insert(float x, float y);

private:
	Rect rect;
	std::list<Triangle> triangle;
};

