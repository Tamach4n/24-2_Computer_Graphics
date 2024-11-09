#include "Planet.h"

Planet::Planet(float r, float degree)
{
	std::cout << "Planet(float r, float degree)" << '\n';
	std::uniform_real_distribution<float> urd(0.f, 360.f);

	shapeAngle = urd(rd);
	std::cout << shapeAngle << '\n';

	pos.x = orbitRadius * cos(glm::radians(shapeAngle));
	pos.y = 0.f;
	pos.z = orbitRadius * sin(glm::radians(shapeAngle));
	orbitRadius = r;
	orbitSpeed = 5.f;
	deg.z = degree;
	rotateY = false;
	rotateX = false;
	
	satellite = new Satellite(*this, 0.5f, -degree);
	satellite->initVerts(0.1f, Position(0.f, 0.f, 1.f));
}

Planet::~Planet()
{
	delete shapeVertex;
	delete satellite;
}

void Planet::initOrbitVerts(const Position& center)
{
	std::vector<float> VAO;

	for (float i = 0.f; i < 360; ++i) {
		VAO.push_back(orbitRadius * cos(glm::radians(i)));
		VAO.push_back(0.f);
		VAO.push_back(orbitRadius * sin(glm::radians(i)));
		VAO.push_back(1.f);
		VAO.push_back(1.f);
		VAO.push_back(1.f);
	}

	orbitVertex = new Vertex(VAO);
}

void Planet::Update(const Position& center)
{
	if (moveX) {
		if (moveX == 1)
			dir.x += 0.01f;

		else
			dir.x -= 0.01f;
	}

	if (moveY == 1) {
		if (moveY == 1)
			dir.y += 0.01f;

		else
			dir.y -= 0.01f;
	}

	if (moveZ) {
		if (moveZ == 1)
			dir.z += 0.01f;

		else
			dir.z -= 0.01f;
	}

	shapeAngle += orbitSpeed;
	pos.x = orbitRadius * cos(glm::radians(shapeAngle));
	//pos.y = center.y;
	pos.z = orbitRadius * sin(glm::radians(shapeAngle));

	if (rotateY) {
		deg.y += 5.f;
	}

	if (rotateX) {
		deg.x += 5.f;
	}

	if (satellite) {
		satellite->Update(*this);
	}
}

void Planet::Draw(GLuint shaderProgram, const Position& center)
{
	GLuint uLoc = glGetUniformLocation(shaderProgram, "modelTransform");
	glm::mat4 STR = glm::mat4(1.f);

	if (uLoc < 0)
		std::cout << "uLoc not found" << '\n';

	else {
		glm::mat4 Rx = glm::rotate(STR, glm::radians(deg.x), glm::vec3(1.f, 0.f, 0.f));
		glm::mat4 Ry = glm::rotate(STR, glm::radians(deg.y), glm::vec3(0.f, 1.f, 0.f));
		glm::mat4 Rz = glm::rotate(STR, glm::radians(deg.z), glm::vec3(0.f, 0.f, 1.f));
		glm::mat4 T = glm::translate(STR, glm::vec3(center.x, center.y, center.z));
		glm::mat4 S;

		{
			STR = T * Ry * Rz * Rx;
			glUniformMatrix4fv(uLoc, 1, GL_FALSE, glm::value_ptr(STR));

			orbitVertex->setActive();
			glDrawArrays(GL_LINE_LOOP, 0, orbitVertex->getNumIndices());
		}

		STR = glm::mat4(1.f);
		Rx = glm::rotate(STR, glm::radians(deg.x), glm::vec3(1.f, 0.f, 0.f));	//	radians(degree) : degree to radian
		Ry = glm::rotate(STR, glm::radians(deg.y), glm::vec3(0.f, 1.f, 0.f));
		Rz = glm::rotate(STR, glm::radians(deg.z), glm::vec3(0.f, 0.f, 1.f));
		T = glm::translate(STR, glm::vec3(pos.x, pos.y, pos.z));
		glm::mat4 T2 = glm::translate(STR, glm::vec3(center.x, center.y, center.z));
		S = glm::scale(STR, glm::vec3(0.5f));

		{
			STR = T2 * Ry * Rz * Rx * T * S;
			glUniformMatrix4fv(uLoc, 1, GL_FALSE, glm::value_ptr(STR));

			shapeVertex->setActive();
			glDrawElements(GL_TRIANGLES, shapeVertex->getNumIndices(), GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));
		}

		STR /= S;
	}	

	if(satellite)
		satellite->Draw(shaderProgram, STR, this->pos);
}

void Planet::setRotateY()
{
	rotateY = !rotateY;

	if (satellite)
		satellite->setRotateY();
}

void Planet::setRotateZ()
{
	rotateX = !rotateX;

	if (satellite)
		satellite->setRotateZ();
}
