#include <iostream>
#include <fstream>
#include <chrono>
#include "Scene.h"

Scene::Scene(int winWidth, int winHeight)
	: width{ winWidth }, height{ winHeight }
{
	hsr = true;
	drawMode = true;
	r = g = b = 0.9f;
}

bool Scene::initialize()
{
	if (!loadShaders()) {
		std::cerr << "Failed to Load Shaders." << '\n';
		return false;
	}

	shapes.push_back(Shape());
	shapes.push_back(Shape(0.5f, 0.0f));
	shapes.push_back(Shape(-0.5f, 0.0f));

	shapes[0].initAxisVerts();
	shapes[1].initConeVerts();
	shapes[2].initCubeVerts();

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	return true;
}

void Scene::update()
{
	for (int i = 0; i < shapes.size(); ++i)
		shapes[i].Update();
}

void Scene::draw()
{
	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	spriteShader->setActive();

	GLuint uLoc = glGetUniformLocation(spriteShader->GetshaderProgram(), "modelTransform");

	if (uLoc < 0)
		std::cout << "uLoc not found" << '\n';

	else {
		for (int i = 0; i < shapes.size(); ++i) {
			shapes[i].setActive(spriteShader);
			shapes[i].Draw();
		}
	}
}

void Scene::keyboard(unsigned char key)
{
	switch (key) {
	case '1':
		std::cout << "1" << "\n";
		shapes[1].setMode(1);
		shapes[2].setMode(1);
		shapes.push_back(Shape());
		shapes[3].initSpiralVerts();
		break;

	case '2':
		std::cout << "2" << "\n";
		shapes[1].setState(false);
		shapes[2].setState(true);
		break;

	case '3':
		std::cout << "3" << "\n";
		shapes[1].setState(true);
		shapes[2].setState(true);
		break;

	case '4':

		break;

	case '5':
		break;

	case 'x':
		shapes[0].setState(false);
		shapes[1].setMawari(true);
		shapes[2].setMawari(true);
		shapes[1].setRotation(1, 0);
		shapes[2].setRotation(1, 0);
		break;

	case 'X':
		shapes[0].setState(false);
		shapes[1].setMawari(true);
		shapes[2].setMawari(true);
		shapes[1].setRotation(-1, 0);
		shapes[2].setRotation(-1, 0);
		break;

	case 'y':
		shapes[0].setState(false);
		shapes[1].setMawari(true);
		shapes[2].setMawari(true);
		shapes[1].setRotation(0, 1);
		shapes[2].setRotation(0, 1);
		break;

	case 'Y':
		shapes[0].setState(false);
		shapes[1].setMawari(true);
		shapes[2].setMawari(true);
		shapes[1].setRotation(0, -1);
		shapes[2].setRotation(0, -1);
		break;

	case 'r':
		for (int i = 0; i < shapes.size(); ++i) {
			shapes[i].setState(true);
			shapes[i].setMawari(false);
			shapes[i].setRotation(0, 1);
		}

		break;

	case 'R':
		for (int i = 0; i < shapes.size(); ++i) {
			shapes[i].setState(true);
			shapes[i].setMawari(false);
			shapes[i].setRotation(0, -1);
		}

		break;

	case 'c':
		shapes[1].changeShape();
		shapes[2].changeShape();
		break;

	case 's':
		shapes[0].setPos(0.f, 0.f);
		shapes[1].setPos(0.5f, 0.0f);
		shapes[2].setPos(-0.5f, 0.0f);
		shapes[0].Reset();
		shapes[1].Reset();
		shapes[2].Reset();
		break;
	}
}

void Scene::keyboardUp(unsigned char key)
{
}

void Scene::specialKeyboard(int key)
{
	switch (key) {
	case GLUT_KEY_UP:
		std::cout << "Up" << '\n';
		break;

	case GLUT_KEY_DOWN:
		std::cout << "Down" << '\n';
		break;

	case GLUT_KEY_LEFT:
		std::cout << "Left" << '\n';
		break;

	case GLUT_KEY_RIGHT:
		std::cout << "Right" << '\n';
		break;
	}
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

bool Scene::loadShaders()
{
	spriteShader = new Shader();

	if (!spriteShader->Load("C:\\Users\\worl\\source\\repos\\Computer_Graphics\\vertex.vert", "C:\\Users\\worl\\source\\repos\\Computer_Graphics\\fragment.frag"))
		return false;

	spriteShader->setActive();

	return true;
}