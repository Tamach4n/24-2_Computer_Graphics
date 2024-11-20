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

	camPos[0] = glm::vec3(-2.f, 2.f, 2.f);
	camPos[1] = glm::vec3( 0.f, 2.f, 0.f);
	camPos[2] = glm::vec3(-2.f, 0.f, 2.f);

	camDeg = 135.f;
	camRad = glm::length(camPos[0]);

	/*camPos[0].x = camRad * cos(glm::radians(camDeg));
	camPos[1].x = camRad * cos(glm::radians(camDeg));
	camPos[2].x = camRad * cos(glm::radians(camDeg));
	camPos[0].z = camRad * sin(glm::radians(camDeg));
	camPos[1].z = camRad * sin(glm::radians(camDeg));
	camPos[2].z = camRad * sin(glm::radians(camDeg));*/

	glm::vec3 camAt = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 camUp = glm::vec3(0.f, 1.f, 0.f);
	glm::vec3 camUp_z = glm::vec3(0.f, 0.f, 1.f);
	
	camDir[0] = glm::normalize(camPos[0] - camAt);
	camDir[1] = glm::normalize(camPos[1] - camAt);
	camDir[2] = glm::normalize(camPos[2] - camAt);

	camU[0] = glm::normalize(glm::cross(camUp, camDir[0]));
	camU[1] = glm::normalize(glm::cross(camUp_z, camDir[1]));
	camU[2] = glm::normalize(glm::cross(camUp, camDir[2]));

	camV[0] = glm::cross(camDir[0], camU[0]);
	camV[1] = glm::cross(camDir[1], camU[1]);
	camV[2] = glm::cross(camDir[2], camU[2]);

	plat = new Shape();
	plat->initPlatBuffer();
	crane = new Shape();
	crane->initBuffer();

	return true;
}

void Scene::update()
{
	crane->Update();

	if (movCamPosiX) {
		camPos[0].x += 0.01f;
		camPos[1].x += 0.01f;
		camPos[2].x += 0.01f;
	}

	else if (movCamNegaX) {
		camPos[0].x -= 0.01f;
		camPos[1].x -= 0.01f;
		camPos[2].x -= 0.01f;
	}

	if (movCamPosiZ) {
		camPos[0].z += 0.01f;
		camPos[1].z += 0.01f;
		camPos[2].z += 0.01f;
	}

	else if (movCamNegaZ) {
		camPos[0].z -= 0.01f;
		camPos[1].z -= 0.01f;
		camPos[2].z -= 0.01f;
	}

	//camRad = glm::length(camPos);

	//	°øÀü
	if (rotCamPosiCenter) {
		/*glm::vec3 ori = camPos - camDir;*/
		camDeg += 3.f;
		camPos[0].x = camRad * cos(glm::radians(camDeg));
		camPos[1].x = camRad * cos(glm::radians(camDeg));
		camPos[2].x = camRad * cos(glm::radians(camDeg));
		camPos[0].z = camRad * sin(glm::radians(camDeg));
		camPos[1].z = camRad * sin(glm::radians(camDeg));
		camPos[2].z = camRad * sin(glm::radians(camDeg));
		/*camDir = glm::normalize(camPos - ori);
		camU = glm::normalize(glm::cross(camDir, glm::vec3(0.f, 1.f, 0.f)));
		camV = glm::cross(camDir, camU);*/
	}

	else if (rotCamNegaCenter) {
		camDeg -= 3.f;
		camPos[0].x = camRad * cos(glm::radians(camDeg));
		camPos[1].x = camRad * cos(glm::radians(camDeg));
		camPos[2].x = camRad * cos(glm::radians(camDeg));
		camPos[0].z = camRad * sin(glm::radians(camDeg));
		camPos[1].z = camRad * sin(glm::radians(camDeg));
		camPos[2].z = camRad * sin(glm::radians(camDeg));
	}

	//	ÀÚÀü
	if (rotCamPosiSelf) {
		glm::mat4 R = glm::rotate(glm::mat4(1.f), glm::radians(30.f), camV[0]);
		camDir[0] = glm::vec3(R * glm::vec4(camDir[0], 1.f));
		camU[0] = glm::normalize(glm::cross(camDir[0], camV[0]));

		R = glm::rotate(glm::mat4(1.f), glm::radians(30.f), camV[1]);
		camDir[1] = glm::vec3(R * glm::vec4(camDir[1], 1.f));
		camU[1] = glm::normalize(glm::cross(camDir[1], camV[1]));

		R = glm::rotate(glm::mat4(1.f), glm::radians(30.f), camV[2]);
		camDir[2] = glm::vec3(R * glm::vec4(camDir[2], 1.f));
		camU[2] = glm::normalize(glm::cross(camDir[2], camV[2]));
	}

	else if (rotCamNegaSelf) {
		glm::mat4 R = glm::rotate(glm::mat4(1.f), glm::radians(-30.f), camV[0]);
		camDir[0] = glm::vec3(R * glm::vec4(camDir[0], 1.f));
		camU[0] = glm::normalize(glm::cross(camDir[0], camV[0]));

		R = glm::rotate(glm::mat4(1.f), glm::radians(-30.f), camV[1]);
		camDir[1] = glm::vec3(R * glm::vec4(camDir[1], 1.f));
		camU[1] = glm::normalize(glm::cross(camDir[1], camV[1]));

		R = glm::rotate(glm::mat4(1.f), glm::radians(-30.f), camV[2]);
		camDir[2] = glm::vec3(R * glm::vec4(camDir[2], 1.f));
		camU[2] = glm::normalize(glm::cross(camDir[2], camV[2]));
	}

	//std::cout << "Length: " << camDeg2 << ", X: " << camPos.x << ", Z: " << camPos.z << '\n';
}

void Scene::draw()
{
	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glViewport(0, 100, 600, 600);
	drawScene(0);

	glViewport(600,600,200,200);
	drawScene(1);
	
	glViewport(600, 0, 200, 200);
	drawScene(2);
}

void Scene::drawScene(int mode)
{
	GLuint viewLoc = glGetUniformLocation(spriteShader->GetshaderProgram(), "viewTransform");
	GLuint projLoc = glGetUniformLocation(spriteShader->GetshaderProgram(), "projTransform");

	if (viewLoc < 0)
		std::cout << "viewLoc not found" << '\n';

	else if (projLoc < 0)
		std::cout << "projLoc not found" << '\n';

	else {
		glm::mat4 view;
		glm::mat4 proj;

		if (mode == 0) {
			view = glm::lookAt(camPos[0], camPos[0] - camDir[0], camV[0]);

			if (Proj)
				proj = glm::perspective(glm::radians(45.f), 1.f, 0.1f, 50.f);

			else
				proj = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -100.f, 100.0f);
		}

		else if (mode == 1) {
			view = glm::lookAt(camPos[1], camPos[1] - camDir[1], camV[1]);
			proj = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -100.f, 100.0f);
		}

		else {
			view = glm::lookAt(camPos[2], camPos[2] - camDir[2], camV[2]);
			proj = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -100.f, 100.0f);
		}

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

		plat->setActive(spriteShader);
		plat->DrawPlat(spriteShader->GetshaderProgram());

		crane->setActive(spriteShader);
		crane->Draw(spriteShader->GetshaderProgram());
	}
}

void Scene::keyboard(unsigned char key)
{
	switch (key) {
	case 'b':
		crane->setMoveX(1);
		break;

	case 'B':
		crane->setMoveX(-1);
		break;


	case 'm':
		crane->setRotateTop(1);
		break;

	case 'M':
		crane->setRotateTop(-1);
		break;


	case 'f':
		crane->setRotateBarrel(1);
		break;

	case 'F':
		crane->setRotateBarrel(-1);
		break;


	case 'e':
		crane->setMoveBarrel(1);
		break;

	case 'E':
		crane->setMoveBarrel(-1);
		break;


	case 't':
		crane->setRotateArm(1);
		break;

	case 'T':
		crane->setRotateArm(-1);
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



	case 'r':
		rotCamPosiSelf = !rotCamPosiSelf;
		rotCamNegaSelf = false;
		break;

	case 'R':
		rotCamPosiSelf = false;
		rotCamNegaSelf = !rotCamNegaSelf;
		break;


	case 'a':
		rotCamPosiSelf = true;
		rotCamNegaSelf = false;
		break;

	case 'A':
		rotCamPosiSelf = false;
		rotCamNegaSelf = true;
		break;


	case 's':
	case 'S':
		crane->setMoveX(0);
		crane->setMoveBarrel(0);
		crane->setRotateArm(0);
		crane->setRotateBarrel(0);
		crane->setRotateTop(0);
		rotCamPosiSelf = false;
		rotCamNegaSelf = false;
		rotCamPosiCenter = false;
		rotCamNegaCenter = false;
		movCamPosiX = false;
		movCamNegaX = false;
		movCamPosiZ = false;
		movCamNegaZ = false;
		break;


	case 'c':
	case 'C':
		crane->init();
		crane->initBuffer();
		rotCamPosiSelf = false;
		rotCamNegaSelf = false;
		rotCamPosiCenter = false;
		rotCamNegaCenter = false;
		movCamPosiX = false;
		movCamNegaX = false;
		movCamPosiZ = false;
		movCamNegaZ = false;
		break;
	}
}

void Scene::keyboardUp(unsigned char key)
{
	if (key == 'r')
		rotCamPosiSelf = false;

	else if (key == 'R')
		rotCamNegaSelf = false;
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