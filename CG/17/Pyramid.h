#include "Shape.h"

class Pyramid : public Shape
{
public:
	void initVerts();

	void Update() override;

	void Draw(GLuint shaderProgram) override;

	void openFace() override;

	void setAnimeMode(int mode) override;

private:
	bool animeSideFaces;
	bool animeOnceFace;
};
