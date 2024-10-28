#pragma once
#include <vector>
#include <gl/glew.h>
#include <iostream>

class Vertex {
public:
	Vertex(const float* verts, unsigned int numVerts, 
		const unsigned int* indices, unsigned int numIndices);

	Vertex(const float* verts, unsigned int numVerts);

	Vertex(const std::vector<float>& verts, const std::vector<unsigned int>& indices);

	Vertex(const std::vector<float>& verts);

	~Vertex();

	void setActive();

	unsigned int getNumIndices()const { return numIndices; }

	unsigned int getNumVerts()const { return numVerts; }


private:
	unsigned int numVerts;

	unsigned int numIndices;

	unsigned int VBO;

	unsigned int indexBuffer;

	unsigned int VAO;
};