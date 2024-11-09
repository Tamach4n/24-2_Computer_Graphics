#pragma once
#include "Shape.h"
#include "Satellite.h"

class Planet : public Shape
{
public:
	Planet(float r, float degree);
	~Planet();

	void initOrbitVerts(const Position& center);

	void Update(const Position& center) override;

	void Draw(GLuint shaderProgram, const Position& center) override;

	void setRotateY() override;
	void setRotateZ() override;

private:
	class Satellite* satellite;
};

