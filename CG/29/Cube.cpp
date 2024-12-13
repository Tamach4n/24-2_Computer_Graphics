#include "Cube.h"
#include "Scene.h"
#include <sstream>
#include <fstream>

Cube::Cube(Scene* scene)
	:Shape(scene)
{
	std::cout << "Cube()" << '\n';
	mTexture.push_back(scene->getTexture("C:\\Users\\worl\\Desktop\\Lecture\\2-2\\CG\\CG\\29\\assets\\A.png"));
	mTexture.push_back(scene->getTexture("C:\\Users\\worl\\Desktop\\Lecture\\2-2\\CG\\CG\\29\\assets\\B.png"));
	mTexture.push_back(scene->getTexture("C:\\Users\\worl\\Desktop\\Lecture\\2-2\\CG\\CG\\29\\assets\\C.png"));
	mTexture.push_back(scene->getTexture("C:\\Users\\worl\\Desktop\\Lecture\\2-2\\CG\\CG\\29\\assets\\D.png"));
	mTexture.push_back(scene->getTexture("C:\\Users\\worl\\Desktop\\Lecture\\2-2\\CG\\CG\\29\\assets\\E.png"));
	mTexture.push_back(scene->getTexture("C:\\Users\\worl\\Desktop\\Lecture\\2-2\\CG\\CG\\29\\assets\\F.png"));
}

void Cube::Init()
{
	std::vector<float> VAO;
	std::vector<unsigned int> VBO;
	readOBJ("C:\\Users\\worl\\Desktop\\Lecture\\2-2\\CG\\CG\\obj\\cube.obj", VAO, VBO);
	shapeVertex = new Vertex(VAO, VBO);
}

void Cube::Draw(const Shader* shader)
{
	shapeVertex->setActive();

	glm::mat4 Rx = glm::rotate(glm::mat4(1.f), glm::radians(deg.x), glm::vec3(1.f, 0.f, 0.f));	//	radians(degree) : degree to radian
	glm::mat4 Ry = glm::rotate(glm::mat4(1.f), glm::radians(deg.y), glm::vec3(0.f, 1.f, 0.f));
	glm::mat4 Rz = glm::rotate(glm::mat4(1.f), glm::radians(deg.z), glm::vec3(0.f, 0.f, 1.f));
	glm::mat4 T = glm::translate(glm::mat4(1.f), glm::vec3(pos.x, pos.y, pos.z));
	glm::mat4 S = glm::scale(glm::mat4(1.f), glm::vec3(mScale));
	glm::mat4 SRT = Rz * Ry * Rx * T * S;

	shader->setMatrixUniform("modelTransform", SRT);
	mTexture[4]->setActive();
	for (int i = 0; i < 6; ++i) {
		if (mTexture[i])
			mTexture[i]->setActive();

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, reinterpret_cast<void*>(sizeof(float) * i * 6));
	}
}