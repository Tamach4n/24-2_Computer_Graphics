#pragma
#include "Shape.h"

class Pyramid : public Shape
{
public:
	Pyramid(Scene* scene);

	void Init() override;

	void Draw(const Shader* shader) override;

private:

};