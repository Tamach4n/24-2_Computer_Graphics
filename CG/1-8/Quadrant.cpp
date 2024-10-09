#include <random>
#include "Quadrant.h"

std::random_device rd;
std::default_random_engine dre(rd());

Quadrant::Quadrant()
{
	rect = {};
}

Quadrant::Quadrant(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
{
	rect = { x1,y1,x2,y2 };
}

void Quadrant::draw(int mode, GLuint pos, GLuint color)
{
	for (const auto& a : triangle) {
		glUniform2f(pos, a.coord.first, a.coord.second);
		glUniform4f(color, a.r, a.g, a.b, 1.0);

		if (mode == 0) {
			glLineWidth(2.f);
			glDrawArrays(GL_LINE_LOOP, 0, 3);
		}

		else
			glDrawArrays(GL_TRIANGLES, 0, 3);
	}
}

void Quadrant::clearShapes()
{
	triangle.clear();
}

bool Quadrant::isClicked(const float& mx, const float& my) const
{
	return (mx > rect.x1 && mx<rect.x2 && my>rect.y1 && my < rect.y2);
}

int Quadrant::getListSize()
{
	return triangle.size();
}

void Quadrant::insert(float x, float y)
{
	std::uniform_real_distribution<float> urd(0.f, 1.f);

	Triangle temp;

	temp.coord = { x,y };
	temp.r = urd(dre);
	temp.g = urd(dre);
	temp.b = urd(dre);

	triangle.push_back(temp);
}
