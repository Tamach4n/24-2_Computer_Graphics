#include "Shape.h"

Shape::Shape()
{
	std::cout << "Shape()" << '\n';
	xPos = yPos = zPos = 0.f;
	xDeg = 5.f;
	yDeg = 5.f;
	zDeg = 0.f;
	xRot = yRot = zRot = 0.f;
	xDir = zDir = 0.f;
	yDir = 5.f;
	isLine = false;
	rotateY = false;
}

Shape::Shape(float x, float y, float z)
	:xPos(x), yPos(y), zPos(z)
{
	std::cout << "Shape(float x, float y)" << '\n';
	xDeg = 0.f;
	yDeg = 0.f;
	zDeg = 0.f;
	xRot = yRot = zRot = 0.f;
	xDir = yDir = zDir = 0.0f;
	isLine = false;
	rotateY = false;
}

void Shape::clearBuffer()
{
	delete shapeVertex;
}

void Shape::initVerts()
{
}

void Shape::initAxisVerts()
{
	float VAO[] = {
		-2.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
		2.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,

		0.0f, -2.0f, 0.0f,  0.0f, 0.0f, 1.0f,
		0.0f, 2.0f, 0.0f,   0.0f, 0.0f, 1.0f,

		0.0f, 0.0f, -2.0f,  0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 2.0f,   0.0f, 1.0f, 0.0f
	};

	isLine = true;
	shapeVertex = new Vertex(VAO, 6);
}

void Shape::setActive(Shader* shader)
{
	shapeVertex->setActive();

	
}

void Shape::setRotateY()
{
	rotateY = !rotateY;
}

void Shape::setPos(float x, float y, float z)
{
	xPos = x;
	yPos = y;
	zPos = z;
}

void Shape::setRotation(float x, float y, float z)
{
	xRot = x;
	yRot = y;
	zRot = z;
}

void Shape::setDirection(float x, float y, float z)
{
	xDir = x;
	yDir = y;
	zDir = z;
}

void Shape::setAnimeMode(int mode)
{
}

void Shape::drawFace(GLuint uLoc, glm::mat4 srt, GLsizei count, int start)
{
}

void Shape::Reset()
{
	xRot = yRot = zRot = 0.f;
	xDir = yDir = zDir = 0.f;
	xDeg = yDeg = zDeg = 0.f;
	rotateY = true;
}

void Shape::rotation()
{
}

void Shape::openFace()
{
}

void Shape::Update()
{
	if (rotateY)
		yDeg += yDir;
}

void Shape::Draw(GLuint shaderProgram)
{
	GLuint uLoc = glGetUniformLocation(shaderProgram, "modelTransform");

	if (uLoc < 0)
		std::cout << "uLoc not found" << '\n';

	else {
		glm::mat4 Rx = glm::rotate(glm::mat4(1.f), glm::radians(xDeg), glm::vec3(1.f, 0.f, 0.f));	//	radians(degree) : degree to radian
		glm::mat4 Ry = glm::rotate(glm::mat4(1.f), glm::radians(yDeg), glm::vec3(0.f, 1.f, 0.f));
		glm::mat4 Rz = glm::rotate(glm::mat4(1.f), glm::radians(zDeg), glm::vec3(0.f, 0.f, 1.f));
		glm::mat4 T = glm::translate(glm::mat4(1.f), glm::vec3(xPos, yPos, zPos));
		glm::mat4 S = glm::scale(glm::mat4(1.f), glm::vec3(0.5f));

		glm::mat4 SRT = T * Rz * Ry * Rx * S;
		glUniformMatrix4fv(uLoc, 1, GL_FALSE, glm::value_ptr(SRT));
	}

	if (isLine)
		glDrawArrays(GL_LINES, 0, shapeVertex->getNumVerts());

	else
		glDrawElements(GL_TRIANGLES, shapeVertex->getNumIndices(), GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));
}
