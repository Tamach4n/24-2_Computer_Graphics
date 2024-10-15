#include "Vertex.h"

Vertex::Vertex(const float* verts, unsigned int numVerts, const unsigned int* indices, unsigned int numIndices)
	:numVerts(numVerts), numIndices(numIndices) 
{
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, numVerts * 6 * sizeof(float), verts, GL_STATIC_DRAW);

	glGenBuffers(1, &indexBuffer);
	//{
	//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);
	//	glEnableVertexAttribArray(0);
	//
	//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(3 * sizeof(float)));
	//	glEnableVertexAttribArray(0);
	//}
	{	//	인덱스 버퍼 방식
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(3 * sizeof(float)));
	}
}

Vertex::Vertex(const std::vector<float>& verts)
	:numVerts(static_cast<unsigned int>(verts.size() / 2))
{
	glGenVertexArrays(1, &vertexArray);	
	glBindVertexArray(vertexArray);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verts.size(), verts.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
	// location, 갯수, 타입, nomalized?, 간격(바이트), 시작오프셋
	glEnableVertexAttribArray(0);
}

Vertex::~Vertex()
{
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &indexBuffer);
	glDeleteVertexArrays(1, &vertexArray);
}

void Vertex::setActive()
{
	glBindVertexArray(vertexArray);
}
