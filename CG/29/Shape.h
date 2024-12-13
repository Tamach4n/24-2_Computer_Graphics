#pragma once
#include <vector>
#include "gl/glew.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "Vertex.h"
#include "Shader.h"

class Shape
{
public:
	Shape(class Scene* scene);
	~Shape() { delete shapeVertex; }

	virtual void Init();
	virtual void readOBJ(const std::string& fileName, std::vector<float>& vao, std::vector<unsigned int>& vbo);

	virtual void setTexture(class Texture* texture) { mTexture.push_back(texture); }

	virtual void setActive();
	virtual void setPosition(glm::vec3 p);
	virtual void setRotation(float x, float y, float z);
	virtual void setDirection(float x, float y, float z);
	virtual void setDegree(glm::vec3 degree) { deg = degree; }
	virtual void setScale(float scale) { mScale = scale; }
	virtual void setOrbit(bool orbit) { mOrbit = orbit; }
	virtual void Reset();

	virtual glm::vec3 getPosition() const { return pos; }
	virtual glm::vec3 getRotation() const { return deg; }

	virtual void Update();

	virtual void Draw(const Shader* shader);

protected:
	Vertex* shapeVertex;
	glm::vec3 pos;
	glm::vec3 deg;

	glm::vec3 dir;
	glm::vec3 rot;	//	x는 아래로, y는 시계 반대 방향으로 회전

	float mScale;
	bool mOrbit = false;

	class Scene* mScene;
	std::vector<class Texture*> mTexture;
};

