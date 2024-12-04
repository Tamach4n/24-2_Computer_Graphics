#include "Shape.h"

Shape::Shape()
{
	std::cout << "Shape()" << '\n';
	pos = deg = dir = rot = glm::vec3(0.f);
	mScale = 0.5f;
}

void Shape::Init()
{	
}

void Shape::setActive()
{
	shapeVertex->setActive();
}

void Shape::setPosition(glm::vec3 p)
{
	pos = p;
}

void Shape::setRotation(float x, float y, float z)
{
	rot.x = x;
	rot.y = y;
	rot.z = z;
}

void Shape::setDirection(float x, float y, float z)
{
	dir.x = x;
	dir.y = y;
	dir.z = z;
}

void Shape::setOrbitPos(const glm::vec3& oPos)
{
	mRadius = glm::length(oPos);
	pos.x = mRadius * cos(glm::radians(deg.y));
	pos.z = mRadius * sin(glm::radians(deg.y));
	std::cout << pos.x << " " << pos.y << " " << pos.z << '\n';
}

void Shape::setRadius(const float& r)
{
	mRadius += r;
	setOrbitPos(pos);
}

void Shape::Reset()
{
	pos = deg = dir = rot = glm::vec3(0.f);
	mScale = 0.5f;
}

void Shape::Update()
{
	pos += dir;
	deg += rot;

	/*if (mOrbit)
		setOrbitPos(pos);*/
}

void Shape::Draw(const Shader* shaderProgram)
{
	shapeVertex->setActive();

	glm::mat4 Rx = glm::rotate(glm::mat4(1.f), glm::radians(deg.x), glm::vec3(1.f, 0.f, 0.f));	//	radians(degree) : degree to radian
	glm::mat4 Ry = glm::rotate(glm::mat4(1.f), glm::radians(deg.y), glm::vec3(0.f, 1.f, 0.f));
	glm::mat4 Rz = glm::rotate(glm::mat4(1.f), glm::radians(deg.z), glm::vec3(0.f, 0.f, 1.f));
	glm::mat4 T = glm::translate(glm::mat4(1.f), glm::vec3(pos.x, pos.y, pos.z));
	glm::mat4 S = glm::scale(glm::mat4(1.f), glm::vec3(mScale));
	glm::mat4 SRT;

	//if(mOrbit)
	SRT = Rz * Ry * Rx * T * S;

	/*else
		SRT = T * Rz * Ry * Rx * S;*/

	shaderProgram->setMatrixUniform("modelTransform", SRT);
	glDrawElements(GL_TRIANGLES, shapeVertex->getNumIndices(), GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));
}
