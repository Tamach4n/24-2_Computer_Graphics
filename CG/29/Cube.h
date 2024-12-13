#pragma
#include "Shape.h"

class Cube : public Shape
{
public:
	Cube(class Scene* scene);

	void Init() override;

	void Draw(const Shader* shader) override;

private:
	
};