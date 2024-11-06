#pragma once
#include "Shape.h"

class Cube : public Shape
{
public:
	Cube();
	void initVerts();

	void Update() override;

	void Draw(GLuint shaderProgram) override;
	void drawFace(GLuint uLoc, glm::mat4 srt, GLsizei count, int start) override;

	void openFace() override;
	
	void setAnimeMode(int mode) override;

private:
	bool animeTopFace;
	bool animeFrontFace;
	bool animeSideFace;
	bool animeBackFace;

	bool isFrontOpened;
	bool isSideOpened;
	bool isBackOpened;

	float topAngle;
	float frontAngle;
	float sideDis;
	float backAngle;
};

