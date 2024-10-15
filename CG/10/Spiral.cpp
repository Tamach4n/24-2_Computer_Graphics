#include "Spiral.h"
#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

Spiral::Spiral()
{
	curVerts = 0;
}

Spiral::Spiral(const Spiral& other)
{
	// 필요한 복사 로직 추가
	this->curVerts = other.curVerts;
}

void Spiral::update(int numVerts)
{
	if (curVerts < numVerts);
	++curVerts;
}

void Spiral::draw(Shader* shader, int mode) const
{	
	// Draw the points
	GLuint color = glGetUniformLocation(shader->GetshaderProgram(), "vColor");
	glUniform4f(color, 0.5f, 0.5f, 0.5f, 1.f);

	if (mode == 0)
		glDrawElements(GL_POINTS, curVerts * 2, GL_UNSIGNED_INT, nullptr);

	else
		glDrawElements(GL_LINE_STRIP, curVerts, GL_UNSIGNED_INT, nullptr);

	/* {
		//glm::mat4 scaleMat = glm::mat4();
		//shader->setUniform("uPos", x, y);
		GLuint loc = glGetUniformLocation(shader->GetshaderProgram(), "uPos");
		//GLuint color = glGetUniformLocation(shader->GetshaderProgram(), "vColor");
		if (loc < 0)
			std::cerr << "Error: Not Found uPos in uniform\n";

		glUniform2f(loc, x, y);

		glDrawElements(, 3, GL_UNSIGNED_INT, nullptr);
	}*/
}

void Spiral::setMode(int mode)
{

}
