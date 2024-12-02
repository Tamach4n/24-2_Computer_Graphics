#include "Satellite.h"
//#include "Planet.h"

Satellite::Satellite(const class Planet& p, float r, float degree)
{
	std::cout << "Satellite(float r, float degree)" << '\n';
	std::uniform_real_distribution<float> urd(0.f, 360.f);

	shapeAngle = urd(rd);
	Position pPos = p.getPos();
	pos.x = pPos.x + orbitRadius* cos(glm::radians(shapeAngle));
	pos.y = 0.f;
	pos.z = pPos.z + orbitRadius * sin(glm::radians(shapeAngle));
	orbitRadius = r;
	orbitSpeed = 0.5f;
	deg.z = degree;
	rotateY = false;
	rotateX = false;

	initOrbitVerts(pPos);
}

void Satellite::initOrbitVerts(const Position& center)
{
	std::vector<float> VAO;
	std::cout << center.x << " " << center.y << " " << center.z << '\n';

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

void Satellite::Update(const Planet& pla)
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

	Position p = pla.getPos();
	Position d = pla.getDeg();

	shapeAngle += orbitSpeed;
	pos.x = orbitRadius * cos(shapeAngle);
	pos.z = orbitRadius * sin(shapeAngle);

	if (rotateY) {
		deg.y = 5.f;
	}

	if (rotateX) {
		deg.x -= 5.f;
	}
}

void Satellite::Draw(GLuint shaderProgram, const glm::mat4& pMat, const Position& pPos)
{
	GLuint uLoc = glGetUniformLocation(shaderProgram, "modelTransform");
	glm::mat4 STR = glm::mat4(1.f);

	if (uLoc < 0)
		std::cout << "uLoc not found" << '\n';

	else {
		glm::mat4 Rx = glm::rotate(STR, glm::radians(deg.x), glm::vec3(1.f, 0.f, 0.f));
		glm::mat4 Ry = glm::rotate(STR, glm::radians(deg.y), glm::vec3(0.f, 1.f, 0.f));
		glm::mat4 Rz = glm::rotate(STR, glm::radians(deg.z), glm::vec3(0.f, 0.f, 1.f));
		glm::mat4 T = glm::translate(STR, glm::vec3(pPos.x, pPos.y, pPos.z));
		glm::mat4 S = glm::scale(STR, glm::vec3(2.0f));

		{
			STR = pMat * Ry * Rz * Rx;
			glUniformMatrix4fv(uLoc, 1, GL_FALSE, glm::value_ptr(STR));

			orbitVertex->setActive();
			glDrawArrays(GL_LINE_LOOP, 0, orbitVertex->getNumIndices());
		}

		STR = glm::mat4(1.f);
		Rx = glm::rotate(STR, glm::radians(deg.x), glm::vec3(1.f, 0.f, 0.f));	//	radians(degree) : degree to radian
		Ry = glm::rotate(STR, glm::radians(deg.y), glm::vec3(0.f, 1.f, 0.f));
		Rz = glm::rotate(STR, glm::radians(deg.z), glm::vec3(0.f, 0.f, 1.f));
		T = glm::translate(STR, glm::vec3(pos.x, pos.y, pos.z));
		S = glm::scale(STR, glm::vec3(0.5f));

		{
			STR = T * pMat * Ry * Rz * Rx * S;
			glUniformMatrix4fv(uLoc, 1, GL_FALSE, glm::value_ptr(STR));

			shapeVertex->setActive();
			glDrawElements(GL_TRIANGLES, shapeVertex->getNumIndices(), GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));
		}
	}
}