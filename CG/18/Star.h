#pragma once
#include "Shape.h"

class Star : public Shape
{
public:
	Star(float r, float degree);
	~Star();

	void Draw(GLuint shaderProgram, const Position& center) override;

	//void Update(const Position& center) override;

private:

};

