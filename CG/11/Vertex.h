#pragma once
#include <vector>
#include <GL/glew.h>
#include <iostream>

class Vertex {
public:
	Vertex(const float* verts, unsigned int numVerts, 
		const unsigned int* indices, unsigned int numIndices);

	Vertex(const std::vector<float>& verts, const std::vector<unsigned int>& indices);

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