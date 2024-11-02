#pragma once
#include "Shape.h"

class Cube : public Shape
{
public:
	void initVerts();

	void Update() override;

	void Draw() override;

	void openFace() override;

private:
	bool animeUpFace;
	bool animeFrontFace;
	bool animeSideFace;
	bool animeBackFace;
};

