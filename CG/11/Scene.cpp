#include <iostream>
#include <fstream>
#include <chrono>
#include "Scene.h"

#define _USE_MATH_DEFINES
#include <math.h>

Scene::Scene(int winWidth, int winHeight)
	: width{ winWidth }, height{ winHeight }
{

	r = g = b = 0.9f;
}

bool Scene::initialize()
{
	if (!loadShaders()) {
		std::cerr << "Failed to Load Shaders." << '\n';
		return false;
	}

	createSpriteVerts();

	return true;
}

void Scene::update()
{
	for (int i = 0; i < shapesVec.size(); ++i) {
		shapesVec[i].Update(spriteShader);
	}
}

void Scene::draw()
{
	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	spriteShader->setActive();
	//spriteVertex->setActive();

	for (int i = 0; i < shapesVec.size(); ++i) {
		shapesVec[i].Draw(spriteShader);
	}
}

void Scene::keyboard(unsigned char key)
{
	switch (key) {
	case 'l':
		std::cout << "L: LINE TO TRIANGLE" << '\n';

		if (shapesVec.empty()) {

		}

		else {
			for (int i = 0; i < shapesVec.size(); ++i) {
				shapesVec[i].setVerts(2);
				shapesVec[i].initBuffer();
			}
		}

		break;

	case 't':
		std::cout << "T: TRIANGLE TO RECTANGLE" << '\n';

		break;

	case'r':	
		std::cout << "R: RECTANGLE TO PENTAGON" << '\n';

		break;

	case 'p':
		std::cout << "P: PENTAGON TO LINE" << '\n';

		break;

	case'a':
		break;
	}
}

void Scene::keyboardUp(unsigned char key)
{
}

void Scene::specialKeyboard(int key)
{
}

void Scene::specialKeyboardUp(int key)
{
}

void Scene::mouse(int button, int state, int x, int y)
{
	// È­¸é ¾÷µ¥ÀÌÆ®°¡ µÈ´Ù....

	constexpr int WHEEL_UP = 3;
	constexpr int WHEEL_DOWN = 4;

	float mx;
	float my;

	switch (state) {
	case GLUT_DOWN:
		switch (button) {
		case GLUT_LEFT_BUTTON:
			mx = (static_cast<float>(x) / width * 2.f - 1.f);
			my = (static_cast<float>(height - y) / height * 2.f - 1.f);

			std::cout << "ÁÂÅ¬¸¯ : " << x << ", " << y << std::endl;
			std::cout << "OpenGL x ÁÂÇ¥´Â " << mx << std::endl;
			std::cout << "OpenGL y ÁÂÇ¥´Â " << my << std::endl;
			break;

		case GLUT_MIDDLE_BUTTON:
			std::cout << "ÈÙÅ¬¸¯ : " << x << ", " << y << std::endl;
			break;

		case GLUT_RIGHT_BUTTON:
			std::cout << "¿ìÅ¬¸¯ : " << x << ", " << y << std::endl;
			break;

		case WHEEL_UP:
			std::cout << "ÈÙ  ¾÷ : " << x << ", " << y << std::endl;
			break;

		case WHEEL_DOWN:
			std::cout << "ÈÙ´Ù¿î : " << x << ", " << y << std::endl;
			break;
		}

		break;

	case GLUT_UP:
		switch (button) {
		case GLUT_LEFT_BUTTON:
			break;

		case GLUT_RIGHT_BUTTON:
			break;

		case GLUT_MIDDLE_BUTTON:
			break;
		}

		break;
	}
}

void Scene::setWindowSize(int winWidth, int winHeight)
{
	width = winWidth;
	height = winHeight;
}

void Scene::randomRGB()
{
	std::uniform_real_distribution<float> urd{ 0.f, 1.f };

	r = urd(rd);
	g = urd(rd);
	b = urd(rd);
}

//	---------------------------------
void Scene::createSpriteVerts()
{
	//std::vector<float> VAO;
	//std::vector<unsigned int> VBO;

	//shape.initBuffer(VAO, VBO);

	//spriteVertex = new Vertex(VAO, VBO);

	shapesVec.push_back(Shapes(3, 0.5f, 0.f, 0.3f));
	shapesVec[0].initBuffer();
}
//	---------------------------------

bool Scene::loadShaders()
{
	spriteShader = new Shader();

	if (!spriteShader->Load("C:\\Users\\worl\\Desktop\\Lecture\\2-2\\CG\\CG\\11\\vertex.vert", "C:\\Users\\worl\\Desktop\\Lecture\\2-2\\CG\\CG\\11\\fragment.frag"))
		return false;

	spriteShader->setActive();

	return true;
}