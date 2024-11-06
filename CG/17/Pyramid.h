#include "Shape.h"

class Pyramid : public Shape
{
public:
	Pyramid();
	void initVerts();

	void Update() override;

	void Draw(GLuint shaderProgram) override;
	void drawFace(GLuint uLoc, glm::mat4 srt, GLsizei count, int start) override;

	void openFace() override;

	void setAnimeMode(int mode) override;

private:
	bool animeSideFaces;
	bool animeOnceFace;

	bool isSidesOpened;
	int curSide;

	float angle;
};
