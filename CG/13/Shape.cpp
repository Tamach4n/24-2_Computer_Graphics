#include "Shape.h"

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
		glm::mat4 Rx = glm::rotate(glm::mat4(1.f), glm::radians(10.f), glm::vec3(1.f, 0.f, 0.f));	//	radians(degree) : degree to radian
		glm::mat4 Ry = glm::rotate(glm::mat4(1.f), glm::radians(10.f), glm::vec3(0.f, 1.f, 0.f));
		glm::mat4 S = glm::scale(glm::mat4(1.f), glm::vec3(0.5f));

		glm::mat4 SRR = Ry * Rx * S;

		glUniformMatrix4fv(uLoc, 1, GL_FALSE, glm::value_ptr(SRR));
	}
}

void Shape::Draw(int start, int count)
{
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (void*)(start * sizeof(unsigned int)));
}
