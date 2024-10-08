#include <random>
#include "Quadrant.h"

std::random_device rd;
std::default_random_engine dre(rd());

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

bool Quadrant::isClicked(const float& mx, const float& my)
{
	return 1;
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
