#pragma once
#include "gl/glew.h"
#include "glm.hpp"
#include "Shader.h"
#include "Vertex.h"
#include <vector>
#include <random>

class Shapes
{
public:
	Shapes(int verts, GLfloat xpos, GLfloat ypos, GLint size);

	Shapes(const Shapes& other);

	void Update(Shader* shader);

	void Draw(Shader* shader);

	void initBuffer();

	void setVerts(int v);

	GLint getVerts() const { return verts; }

private:
	Vertex* vertex;

	std::vector<float> VAO;
	std::vector<unsigned int> VBO;

	GLint verts;
	GLint numIndices;
	GLfloat time;
	GLfloat xPos, yPos;
	GLfloat size;
	
	GLclampf r, g, b;

	std::random_device rd;
};

