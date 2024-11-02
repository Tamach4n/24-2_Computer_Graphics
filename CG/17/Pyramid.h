#include "Shape.h"

class Pyramid : public Shape
{
public:
	void initVerts();

	void Update() override;

	void Draw() override;

	void openFace() override;

private:
	bool animeSideFaces;
	bool animeOnceFace;
};
