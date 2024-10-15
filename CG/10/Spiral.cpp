#include "Spiral.h"
#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

Spiral::Spiral()
{
	//std::default_random_engine dre(rd());
	std::uniform_real_distribution<float> urdX{ -0.6f, 0.6f };
	std::uniform_real_distribution<float> urdY{ -0.9f, 0.9f };

	xPos = urdX(rd);
	yPos = urdY(rd);
	std::cout << xPos << " " << yPos << '\n';
	pointVec.push_back({ xPos, yPos });

	curr = 1;
}

Spiral::Spiral(float x, float y)
	:xPos(x), yPos(y)
{
	pointVec.push_back({ xPos, yPos });
	std::cout << xPos << " " << yPos << '\n';
	curr = 1;
}

Spiral::Spiral(const Spiral& other)
{
	// 필요한 복사 로직 추가
	this->xPos = other.xPos;
	this->yPos = other.yPos;
	this->curr = other.curr;
}

void Spiral::update(int size)
{
	if (curr < size)
		++curr;
}

void Spiral::draw(Shader* shader, int mode) const
{
	std::cout << "Spiral::draw()" << '\n' << "Curr: " << curr << '\n';
	std::cout << "X: " << xPos << " Y: " << yPos << '\n';

	// Draw the points
	GLuint color = glGetUniformLocation(shader->GetshaderProgram(), "vColor");
	glUniform3f(color, 0.f, 0.f, 0.f);

	GLuint pos = glGetUniformLocation(shader->GetshaderProgram(), "uPos");
	glUniform2f(pos, xPos, yPos);

	if (mode == 0)
		glDrawArrays(GL_POINTS, 0, curr);

	else
		glDrawArrays(GL_LINE_STRIP, 0, curr);
}
