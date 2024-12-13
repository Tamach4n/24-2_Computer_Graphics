#pragma
#include "Shape.h"

class Cube : public Shape
{
public:
	Cube(class Scene* scene);

	void Init() override;

	void Draw(const Shader* shader) override;
	void DrawBG(const Shader* shader);

private:
	
};