#include "Shapes.h"

Shapes::Shapes(int verts, GLfloat xpos, GLfloat ypos, GLint size)
	:verts(verts), xPos(xpos), yPos(ypos), size(size)
{
	std::cout << "Shapes(int verts, GLfloat xpos, GLfloat ypos, GLint size)" << '\n';

	std::uniform_real_distribution<float> urd(0.f, 1.f);

	time = 0;
	r = urd(rd);
	g = urd(rd);
	b = urd(rd);
}

Shapes::Shapes(const Shapes& other)
{
	std::cout << "Shapes(const Shapes& other)" << '\n';

	this->xPos = other.xPos;
	this->yPos = other.yPos;
	this->r = other.r;
	this->g = other.g;
	this->b = other.b;
	this->size = other.size;
	this->VAO = other.VAO;
	this->VBO = other.VBO;
	this->verts = other.verts;
	this->vertex = other.vertex;
	this->numIndices = other.numIndices;
	this->time = other.time;
}

void Shapes::Update(Shader* shader)
{
	if (time <= 1.f)
		time += 0.01f;
	
	else
		time = 0.f;
	

	initBuffer();

	if (verts >= 6)
		return;

	std::cout << "Shapes::Update(), time: " << time << '\n';
}

void Shapes::Draw(Shader* shader)
{
	//std::cout << "Shapes::Draw(Shader* shader)" << '\n';
	std::cout << "X: " << xPos << " Y: " << yPos << '\n';
	vertex->setActive();

	GLuint Pos = glGetUniformLocation(shader->GetshaderProgram(), "uPos");
	GLuint Color = glGetUniformLocation(shader->GetshaderProgram(), "uColor");

	glUniform2f(Pos, xPos, yPos);
	glUniform3f(Color, r, g, b);

	if (verts == 0) {
		glDrawElements(GL_POINT, numIndices, GL_UNSIGNED_INT, 0);
	}

	else if (verts == 1 || (verts == 2 && time == 0.0f)) {
		glDrawElements(GL_LINES, numIndices, GL_UNSIGNED_INT, 0);
	}

	else 
		glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);

	//glDrawElements(GL_LINE_LOOP, numIndices, GL_UNSIGNED_INT, 0);
}

void Shapes::initBuffer()
{
	std::vector<float> s;
	std::vector<float> l;

	float s_rad = 360.0 / verts;
	float l_rad = 360.0 / (verts + 1);

	for (int i = 0; i < verts + 1; ++i) {
		s.push_back(xPos + cos(glm::radians(i * s_rad)) * size);	//x
		s.push_back(yPos + sin(glm::radians(i * s_rad)) * size);	//y

		if (verts < 5) {
			l.push_back(xPos + cos(glm::radians(i * l_rad)) * size);	//x
			l.push_back(yPos + sin(glm::radians(i * l_rad)) * size);	//y
		}

		else {
			l.push_back(xPos);
			l.push_back(yPos);
		}

		VAO.push_back((1 - time) * s.at(static_cast<std::vector<float, std::allocator<float>>::size_type>(i) * 2) + time * l.at(i * 2));			//x
		VAO.push_back((1 - time) * s.at(static_cast<std::vector<float, std::allocator<float>>::size_type>(i) * 2 + 1) + time * l.at(i * 2 + 1));	//y
		VAO.push_back(r);
		VAO.push_back(g);
		VAO.push_back(b);
		
		std::cout << "i: " << i << " Vertex: " << VAO.at(i * 5) << " " << VAO.at(i * 5 + 1) << " " << "Time: " << time << '\n';
	}

	if (verts + 1 > 0)
		VBO.push_back(0);

	if (verts + 1 > 1)
		VBO.push_back(1);

	if (verts + 1 > 2)
		VBO.push_back(2);

	if (verts + 1 > 3) {
		VBO.push_back(0);
		VBO.push_back(2);
		VBO.push_back(3);
	}

	if (verts + 1 > 4) {
		VBO.push_back(0);
		VBO.push_back(3);
		VBO.push_back(4);
	}

	numIndices = VBO.size();

	vertex = new Vertex(VAO, VBO);
}

void Shapes::setVerts(int v)
{
	verts = v;
}
