#include <iostream>
#include <fstream>
#include <chrono>
#include "Scene.h"

Scene::Scene(int winWidth, int winHeight)
	: width{ winWidth }, height{ winHeight }
{
	r = g = b = 0.1f;
}

bool Scene::initialize()
{
	if (!loadShaders()) {
		std::cerr << "Failed to Load Shaders." << '\n';
		return false;
	}

	hsr = true;
	Proj = true;
	polygonMode = true;

	rotCamPosiSelf = false;
	rotCamNegaSelf = false;
	rotCamPosiCenter = false;
	rotCamNegaCenter = false;
	movCamPosiX = false;
	movCamNegaX = false;
	movCamPosiZ = false;
	movCamNegaZ = false;

	camPos = glm::vec3(0.f, 1.f, 5.f);

	camDeg = 90.f;
	camRad = glm::length(camPos);

	camPos.x = camRad * cos(glm::radians(camDeg));
	camPos.z = camRad * sin(glm::radians(camDeg));

	glm::vec3 camAt = glm::vec3(0.f, 1.f, 0.f);
	glm::vec3 camUp = glm::vec3(0.f, 1.f, 0.f);
	
	camDir = glm::normalize(camPos - camAt);

	camU = glm::normalize(glm::cross(camUp, camDir));
	camV = glm::cross(camDir, camU);

	robot = new Robot();
	robot->init();
	robot->initBuffer();

	butai = new Butai();
	
	/*obs = new Cbstackle[3]{ Cbstackle(), Cbstackle(), Cbstackle() };*/

	return true;
}

void Scene::update()
{
	robot->Update();
	butai->Update();

	if (movCamPosiX) {
		camPos.x += 0.01f;
		camRad = glm::length(camPos);
	}

	else if (movCamNegaX) {
		camPos.x -= 0.01f;
		camRad = glm::length(camPos);
	}

	if (movCamPosiZ) {
		camPos.z += 0.01f;
		camRad = glm::length(camPos);
	}

	else if (movCamNegaZ) {
		camPos.z -= 0.01f;
		camRad = glm::length(camPos);
	}

	//	°øÀü
	if (rotCamPosiCenter) {
		/*glm::vec3 ori = camPos - camDir;*/
		camDeg += 3.f;
		camPos.x = camRad * cos(glm::radians(camDeg));
		camPos.z = camRad * sin(glm::radians(camDeg));

		camDir = glm::normalize(camPos - glm::vec3(0.f, 1.f, 0.f));
		camU = glm::normalize(glm::cross(camDir, glm::vec3(0.f, 1.f, 0.f)));
		camV = glm::cross(-camDir, camU);
	}

	else if (rotCamNegaCenter) {
		camDeg -= 3.f;
		camPos.x = camRad * cos(glm::radians(camDeg));
		camPos.z = camRad * sin(glm::radians(camDeg));
	}
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
		glm::mat4 view = glm::lookAt(camPos, camPos - camDir, camV);
		glm::mat4 proj;

		if (Proj)
			proj = glm::perspective(glm::radians(45.f), 1.f, 0.1f, 50.f);

		else
			proj = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -100.f, 100.0f);

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

		robot->setActive(spriteShader);
		robot->Draw(spriteShader->GetshaderProgram());

		butai->setActive(spriteShader);
		butai->Draw(spriteShader->GetshaderProgram());
	}
}

void Scene::keyboard(unsigned char key)
{
	switch (key) {
	case ' ':
		butai->setOpen();
		break;

	case 'w':
		robot->setDir(Dir_Back);
		break;

	case 'a':
		robot->setDir(Dir_Left);
		break;

	case 's':
		robot->setDir(Dir_Front);
		break;

	case 'd':
		robot->setDir(Dir_Right);
		break;

	case '+':
		robot->adjSpeed(1);
		break;

	case '-':
		robot->adjSpeed(-1);
		break;

	case 'j':
		robot->setJump();
		break;

	case 'i':
		break;

	case 'x':
		movCamPosiX = !movCamPosiX;
		movCamNegaX = false;
		break;

	case 'X':
		movCamPosiX = false;
		movCamNegaX = !movCamNegaX;
		break;


	case 'z':
		movCamPosiZ = !movCamPosiZ;
		movCamNegaZ = false;
		break;

	case 'Z':
		movCamPosiZ = false;
		movCamNegaZ = !movCamNegaZ;
		break;


	case 'y':
		rotCamPosiCenter = !rotCamPosiCenter;
		rotCamNegaCenter = false;
		break;

	case 'Y':
		rotCamPosiCenter = false;
		rotCamNegaCenter = !rotCamNegaCenter;
		break;
	}
}

void Scene::keyboardUp(unsigned char key)
{
	if (key == 'w' || key == 'a' || key == 's' || key == 'd')
		robot->setDir(Dir_None);
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

	if (!spriteShader->Load("C:\\Users\\worl\\source\\repos\\Computer_Graphics\\vertex2.vert", "C:\\Users\\worl\\source\\repos\\Computer_Graphics\\fragment.frag"))
		return false;

	spriteShader->setActive();

	return true;
}