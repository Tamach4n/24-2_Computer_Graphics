#include "Vertex.h"

Vertex::Vertex(const float* verts, unsigned int numVerts, const unsigned int* indices, unsigned int numIndices)
	:numVerts(numVerts), numIndices(numIndices)
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, numVerts * 6 * sizeof(float), verts, GL_STATIC_DRAW);

	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6,
		reinterpret_cast<void*>(sizeof(float) * 3));
}

Vertex::Vertex(const float* verts, unsigned int numVerts)
	:numVerts(numVerts), indexBuffer(0), numIndices(0)
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, numVerts * 6 * sizeof(float), verts, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6,
		reinterpret_cast<void*>(sizeof(float) * 3));
}

Vertex::~Vertex()
{
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &indexBuffer);
	glDeleteVertexArrays(1, &VAO);
}

void Vertex::setActive()
{
	glBindVertexArray(VAO);
}
