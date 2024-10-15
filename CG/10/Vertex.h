#pragma once

class Vertex {
public:
	Vertex(const float* verts, unsigned int numVerts, 
		const unsigned int* indices, unsigned int numIndices);

	~Vertex();

	void setActive();

	unsigned int getNumIndices()const { return numIndices; }

	unsigned int getNumVerts()const { return numVerts; }


private:
	unsigned int numVerts;

	unsigned int numIndices;

	unsigned int vertexBuffer;

	unsigned int indexBuffer;

	unsigned int vertexArray;
};