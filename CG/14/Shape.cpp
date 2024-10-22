#include "Shape.h"

Shape::Shape()
{
	std::cout << "Shape()" << '\n';
	xPos = yPos = 0.f;
	xDeg = 30.f;
	yDeg = -30.f;
	xRot = yRot = 0.f;
	xDir = yDir = 0.f;
}

void Shape::Init()
{	
}

void Shape::setActive(Shader* shader)
{
	shapeVertex->setActive();

	GLuint uLoc = glGetUniformLocation(shader->GetshaderProgram(), "modelTransform");

	if (uLoc < 0)
		std::cout << "uLoc not found" << '\n';

	else {
		glm::mat4 Rx = glm::rotate(glm::mat4(1.f), glm::radians(xDeg), glm::vec3(1.f, 0.f, 0.f));	//	radians(degree) : degree to radian
		glm::mat4 Ry = glm::rotate(glm::mat4(1.f), glm::radians(yDeg), glm::vec3(0.f, 1.f, 0.f));
		glm::mat4 Tx = glm::translate(glm::mat4(1.f), glm::vec3(xPos, 0.f, 0.f));
		glm::mat4 Ty = glm::translate(glm::mat4(1.f), glm::vec3(0.f, yPos, 0.f));
		glm::mat4 S = glm::scale(glm::mat4(1.f), glm::vec3(0.5f));

		glm::mat4 SRT = Ty * Tx * Ry * Rx * S;

		glUniformMatrix4fv(uLoc, 1, GL_FALSE, glm::value_ptr(SRT));
	}
}

void Shape::setRotation(float x, float y)
{
	xRot = x;
	yRot = y;
}

void Shape::setDirection(float x, float y)
{
	xDir = x;
	yDir = y;
}

void Shape::Reset()
{
	xPos = yPos = xRot = yRot = xDir = yDir = 0.f;
	//xDeg = 30.f;
	//yDeg = -30.f;
}

void Shape::Update()
{
	xPos += xDir;
	yPos += yDir;
	xDeg += xRot;
	yDeg += yRot;
}

void Shape::Draw()
{
	glDrawElements(GL_TRIANGLES, shapeVertex->getNumIndices(), GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));
}
