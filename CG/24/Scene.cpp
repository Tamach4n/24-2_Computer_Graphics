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

	glm::vec3 camAt = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 camUp = glm::vec3(0.f, 1.f, 0.f);

	camPos = glm::vec3(-1.f, 1.f, 1.f);
	camDir = glm::normalize(camPos - camAt);
	camU = glm::normalize(glm::cross(camUp, camDir));
	camV = glm::cross(camDir, camU);
	
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

	glm::mat4 view = glm::lookAt(camPos, camPos - camDir, camV);
	glm::mat4 proj = glm::perspective(glm::radians(45.f), 1.f, 0.1f, 50.f);

	spriteShader->setMatrixUniform("viewTransform", view);
	spriteShader->setMatrixUniform("projTransform", proj);
	spriteShader->setVec3Uniform("cameraPos", camPos.x, camPos.y, camPos.z);
	spriteShader->setVec3Uniform("lightPos", 0.f, 0.f, 1.f);
	spriteShader->setVec3Uniform("lightColor", 1.f, 1.f, 1.f);

	spriteShader->setActive();
	axisVertex->setActive();

	glDrawArrays(GL_LINES, 0, 6);
	curr->Draw(spriteShader);
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

	case 'y':
		curr->setRotation(0, 1, 0);
		break;

	case 'Y':
		curr->setRotation(0, -1, 0);
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

void Scene::createAxisVerts()
{
	float VAO[] = {
		-1.0f,  0.0f,  0.0f,	1.0f, 0.0f, 0.0f,
		 1.0f,  0.0f,  0.0f,	1.0f, 0.0f, 0.0f,

		 0.0f, -1.0f,  0.0f,	0.0f, 0.0f, 1.0f,
		 0.0f,  1.0f,  0.0f,	0.0f, 0.0f, 1.0f,

		 0.0f,  0.0f, -1.0f,	0.0f, 1.0f, 0.0f,
		 0.0f,  0.0f,  1.0f,    0.0f, 1.0f, 0.0f 
	};

	axisVertex = new Vertex(VAO, 6);
}

bool Scene::loadShaders()
{
	axisShader = new Shader();

	if (!axisShader->Load("C:\\Users\\worl\\Desktop\\Lecture\\2-2\\CG\\CG\\vertex.vert", "C:\\Users\\worl\\Desktop\\Lecture\\2-2\\CG\\CG\\fragment.frag"))
		return false;

	spriteShader = new Shader();

	if (!spriteShader->Load("C:\\Users\\worl\\Desktop\\Lecture\\2-2\\CG\\CG\\vertex3.vert", "C:\\Users\\worl\\Desktop\\Lecture\\2-2\\CG\\CG\\fragment2.frag"))
		return false;

	return true;
}