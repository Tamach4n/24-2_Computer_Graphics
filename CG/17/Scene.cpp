#include <iostream>
#include <fstream>
#include <chrono>
#include "Scene.h"

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

	hsr = true;
	Proj = true;

	shapeMode = 1;
	axisShape = new Shape();
	axisShape->initAxisVerts();
	shape = new Cube();
	shape->initVerts();

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	return true;
}

void Scene::update()
{
	axisShape->Update();
	shape->Update();
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
		glm::mat4 proj = glm::mat4(1.f);

		if (Proj)
			proj = glm::perspective(glm::radians(45.f), 1.f, 0.1f, 50.f);

		else
			proj = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -100.f, 100.0f);

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

		axisShape->setActive(spriteShader);
		axisShape->Draw(spriteShader->GetshaderProgram());
		shape->setActive(spriteShader);
		shape->Draw(spriteShader->GetshaderProgram());
	}
}

void Scene::keyboard(unsigned char key)
{
	switch (key) {
	case '1':
		if (2 == shapeMode) {
			shapeMode = 1;
			delete shape;
			shape = new Cube();
		}

		break;

	case '2':
		if (1 == shapeMode) {
			shapeMode = 2;
			delete shape;
			shape = new Pyramid();
		}

		break;

	case 'h':
		hsr = !hsr; 
		
		if (hsr)
			glEnable(GL_DEPTH_TEST);

		else
			glDisable(GL_DEPTH_TEST);

		break;

	case 'y':
		shape->setRotateY();
		break;

	case 't':
		shape->setAnimeMode(1);
		break;

	case 'f':
		shape->setAnimeMode(2);
		break;

	case 's':
		shape->setAnimeMode(3);
		break;

	case 'b':
		shape->setAnimeMode(4);
		break;

	case 'o':
		shape->setAnimeMode(1);
		break;

	case 'r':
		shape->setAnimeMode(2);
		break;

	case 'p':
		Proj = !Proj;
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

void Scene::removeShape()
{
	delete shape;
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