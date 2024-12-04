#include <iostream>
#include <fstream>
#include <chrono>
#include "Scene.h"

Scene::Scene(int winWidth, int winHeight)
	: width{ winWidth }, height{ winHeight }
{
	index = new int[2] {};
	r = g = b = 0.9f;
}

bool Scene::initialize()
{
	if (!loadShaders()) {
		std::cerr << "Failed to Load Shaders." << '\n';
		return false;
	}

	shapes.clear();
	shapes.push_back(new Shape());
	shapes.push_back(new Shape(1.0f, 0.0f));
	shapes.push_back(new Shape(-1.0f, 0.0f));

	shapes[0]->initAxisVerts();
	shapes[1]->initConeVerts();
	shapes[2]->initCubeVerts();

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	return true;
}

void Scene::update()
{
	for (int i = 0; i < shapes.size(); ++i)
		shapes[i]->Update(index[i]);
}

void Scene::draw()
{
	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLuint viewLoc = glGetUniformLocation(spriteShader->GetshaderProgram(), "viewTransform");
	GLuint projLoc = glGetUniformLocation(spriteShader->GetshaderProgram(), "projTransform");

	if (viewLoc < 0)
		std::cout << "viewLoc not found" << '\n';

	else if (projLoc < 0)
		std::cout << "projLoc not found" << '\n';
	
	else {
		glm::vec3 camPos = glm::vec3(2.f, 2.f, 2.f);
		glm::vec3 camAt = glm::vec3(0.f, 0.f, 0.f);
		glm::vec3 camUp = glm::vec3(0.f, 1.f, 0.f);

		glm::mat4 view = glm::lookAt(camPos, camAt, camUp);
		glm::mat4 proj = glm::perspective(glm::radians(45.f), 1.f, 0.1f, 50.f);

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

		for (int i = 0; i < shapes.size(); ++i) {
			shapes[i]->setActive();
			shapes[i]->Draw();
		}
	}
}

void Scene::keyboard(unsigned char key)
{
	switch (key) {
	case '1':
		std::cout << "Mode 1" << "\n";
		checkSize();
		shapes[1]->Reset();
		shapes[2]->Reset();
		shapes.push_back(new Shape());
		shapes[3]->initSpiralVerts();
		shapes[1]->setSpeed(1.f);
		shapes[2]->setSpeed(4.f);
		shapes[1]->setMode(1);
		shapes[2]->setMode(1);
		break;

	case '2':
		std::cout << "Mode 2" << "\n";
		checkSize();
		shapes[1]->Reset();
		shapes[2]->Reset();
		shapes[1]->setMode(2);
		shapes[2]->setMode(2);
		shapes[1]->setPos(1.f, 0.f, 0.f);
		shapes[2]->setPos(-1.f, 0.f, 0.f);
		shapes[1]->setDirection(-0.05f, 0.f, 0.f);
		shapes[2]->setDirection(0.05f, 0.f, 0.f);
		index[1] = -1;
		index[2] = 1;
		break;

	case '3':
		std::cout << "Mode 3" << "\n";
		checkSize();
		shapes[1]->Reset();
		shapes[2]->Reset();
		shapes[1]->setMode(3);
		shapes[2]->setMode(3);
		shapes[1]->setPos(1.f, 0.f, 0.f);
		shapes[2]->setPos(-1.f, 0.f, 0.f);
		shapes[1]->setRotation(0.f, 2.0f, 0.f);
		shapes[2]->setRotation(0.f, 2.0f, 0.f);
		shapes[1]->setMawari(false);
		shapes[2]->setMawari(false);
		index[1] = 180;
		index[2] = 180;

		break;

	case '4':
		checkSize();
		shapes[1]->Reset();
		shapes[2]->Reset();
		shapes[1]->setMode(4);
		shapes[2]->setMode(4);
		shapes[1]->setPos(1.f, 0.f, 0.f);
		shapes[2]->setPos(-1.f, 0.f, 0.f);
		shapes[1]->setDirection(-0.05f, 0.0375f, 0.f);
		shapes[2]->setDirection(0.05f, -0.0375f, 0.f);
		index[1] = -1;
		index[2] = 1;

		break;

	case '5':
		checkSize();
		shapes[1]->Reset();
		shapes[2]->Reset();
		shapes[1]->setMode(5);
		shapes[2]->setMode(5);
		shapes[1]->setPos(1.f, 0.f, 0.f);
		shapes[2]->setPos(-1.f, 0.f, 0.f);
		shapes[1]->setRotation(0.f, 2.f, 0.f);
		shapes[2]->setRotation(0.f, 2.f, 0.f);
		shapes[1]->setMawari(false);
		shapes[2]->setMawari(true);
		shapes[1]->setSize(0.001f);
		shapes[2]->setSize(-0.001f);
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

void Scene::checkSize()
{
	if (shapes.size() == 4)
		removeShape();
}

void Scene::removeShape()
{
	delete shapes[3];
	shapes.erase(shapes.begin() + 3);
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

	if (!spriteShader->Load("C:\\Users\\worl\\source\\repos\\Computer_Graphics\\vertex2.vert", "C:\\Users\\worl\\source\\repos\\Computer_Graphics\\fragment.frag"))
		return false;

	spriteShader->setActive();

	return true;
}