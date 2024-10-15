#include "Spiral.h"
#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

Spiral::Spiral()
{
	//std::default_random_engine dre(rd());
	std::uniform_real_distribution<float> urd{ -0.5f, 0.5f };

	xPos = urd(rd);
	yPos = urd(rd);

	pointVec.push_back({ xPos, yPos });

	angle = 0.f;
	radius = 0.f;
	angleSpeed = 0.1f;
	radiusGrowth = 0.001f;
	step = false;
}

Spiral::Spiral(const Spiral& other)
{
	// �ʿ��� ���� ���� �߰�
	this->xPos = other.xPos;
	this->yPos = other.yPos;
	this->angle = other.angle;
	this->angleSpeed = other.angleSpeed;
	this->radius = other.radius;
	this->radiusGrowth = other.radiusGrowth;
	this->pointVec = other.pointVec;
	this->step = other.step;
}

void Spiral::update(int mode)
{
	if (pointVec.empty())
		return;

	std::cout << pointVec.back().first << " " << pointVec.back().second << '\n';

	float newX;
	float newY;

	if (!step) {
		angle += angleSpeed;
		radius += radiusGrowth;	

		newX = xPos + radius * cos(angle);
		newY = yPos + radius * sin(angle);
	}

	else {
		angle += angleSpeed;
		radius -= radiusGrowth;

		newX = xPos - radius * cos(angle);
		newY = yPos + radius * sin(angle);
	}
	
	pointVec.push_back({ newX, newY });

	if (angle >= 4 * M_PI) {
		step = true;
		angle = -angle;
		angleSpeed;
		xPos += 2 * radius;
		std::cout << "Radious: "<<radius << '\n';
	}

	else if (step && angle >= 0 * M_PI) {
		std::cout << "2160" << '\n';
		angleSpeed = 0.f;
		radiusGrowth = 0.f;
	}

	std::cout << static_cast<int>(angle * 180 / M_PI) << '\n';
	std::cout << xPos << " " << yPos << '\n';
}

void Spiral::draw(Shader* shader, int mode) const
{
	std::vector<float> vertices;
	for (const auto& p : pointVec) {
		vertices.push_back(p.first);  // x-coordinate
		vertices.push_back(p.second); // y-coordinate
	}

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	// Load the vertex data into the buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	// Specify the layout of the vertex data (x, y positions)
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//glUseProgram(shader->GetshaderProgram());

	// Draw the points
	GLuint color = glGetUniformLocation(shader->GetshaderProgram(), "vColor");
	glUniform4f(color, 0.5f, 0.5f, 0.5f, 1.f);

	if (mode == 0)
		glDrawArrays(GL_POINTS, 0, pointVec.size());

	else
		glDrawArrays(GL_LINE_STRIP, 0, pointVec.size());

	// Clean up
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
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