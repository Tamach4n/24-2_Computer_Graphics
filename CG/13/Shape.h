#pragma once
#
#include "gl/glew.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "Vertex.h"
#include "Shader.h"

class Shape
{
public:
	~Shape() { delete shapeVertex; }

	virtual void Init();

	virtual void setActive(Shader* shader);

	//virtual void Update();

	virtual void Draw(int start, int count);

protected:
	Vertex* shapeVertex;	
};

