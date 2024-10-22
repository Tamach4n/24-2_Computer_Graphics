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
	
	createAxisVerts();

	cube = new Cube;
	cube->Init();
	pyramid = new Pyramid;
	pyramid->Init();
	curr = cube;
	
	return true;
}

void Scene::update()
{
	if (drawMode)	//	0
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (hsr)
		glEnable(GL_DEPTH_TEST);

	else
		glDisable(GL_DEPTH_TEST);

	curr->Update();
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
		axisVertex->setActive();

		glm::mat4 mat(1.f);
		glUniformMatrix4fv(uLoc, 1, GL_FALSE, glm::value_ptr(mat));

		glDrawArrays(GL_LINES, 0, 4);

		curr->setActive(spriteShader);
		curr->Draw();
	}
}

void Scene::keyboard(unsigned char key)
{
	switch (key) {
	case 'c':
		std::cout << "Cube Selected" << '\n';
		curr = cube;
		break;

	case 'p':
		std::cout << "Pyramid Selected" << '\n';
		curr = pyramid;
		break;

	case 'h':
	case 'H':
		hsr = !hsr;
		std::cout << "HSR: " << hsr << '\n';
		break;

	case 'w':
	case 'W':
		drawMode = !drawMode;
		std::cout << "DrawMode: " << drawMode << '\n';
		break;

	case 'x':
		curr->setRotation(1, 0);
		break;

	case 'X':
		curr->setRotation(-1, 0);
		break;

	case 'y':
		curr->setRotation(0, 1);
		break;

	case 'Y':
		curr->setRotation(0, -1);
		break;

	case 's':
		curr->Reset();
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
		curr->setDirection(0.f, 0.005f);
		break;

	case GLUT_KEY_DOWN:
		std::cout << "Down" << '\n';
		curr->setDirection(0.f, -0.005f);
		break;

	case GLUT_KEY_LEFT:
		curr->setDirection(-0.005f, 0.f);
		std::cout << "Left" << '\n';
		break;

	case GLUT_KEY_RIGHT:
		std::cout << "Right" << '\n';
		curr->setDirection(0.005f, 0.f);
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

void Scene::createAxisVerts()
{
	float VAO[] = {
		-1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,

		0.0f, -1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 1.0f,

		0.0f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f 
	};

	axisVertex = new Vertex(VAO, 6);
}

bool Scene::loadShaders()
{
	spriteShader = new Shader();

	if (!spriteShader->Load("C:\\Users\\worl\\source\\repos\\Computer_Graphics\\vertex.vert", "C:\\Users\\worl\\source\\repos\\Computer_Graphics\\fragment.frag"))
		return false;

	spriteShader->setActive();

	return true;
}