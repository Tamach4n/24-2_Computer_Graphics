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

	camPos = glm::vec3(-2.f, 1.f, 5.f);

	camDeg = 90.f;
	camRad = glm::length(camPos);

	camPos.x = camRad * cos(glm::radians(camDeg));
	camPos.z = camRad * sin(glm::radians(camDeg));

	glm::vec3 camAt = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 camUp = glm::vec3(0.f, 1.f, 0.f);
	glm::vec3 camUp_z = glm::vec3(0.f, 0.f, 1.f);
	
	camDir = glm::normalize(camPos - camAt);
	camU = glm::normalize(glm::cross(camUp, camDir));
	camV = glm::cross(camDir, camU);

	plat = new Shape();
	plat->initPlatBuffer();
	crane = new Shape();
	crane->initBuffer();

	lightColor = glm::vec3(1.f);
	lightDeg = 0.f;
	lightPos = glm::vec3(0.f, 2.f, 3.f);
	lightRad = glm::length(lightPos);
	lightPos.x = lightRad * cos(glm::radians(lightDeg));
	lightPos.y = 2.f;
	lightPos.z = lightRad * sin(glm::radians(lightDeg));

	return true;
}

void Scene::update()
{
	crane->Update();

	if (movCamPosiX) {
		camPos.x += 0.01f;
	}

	else if (movCamNegaX) {
		camPos.x -= 0.01f;
	}

	if (movCamPosiZ) {
		camPos.z += 0.01f;
	}

	else if (movCamNegaZ) {
		camPos.z -= 0.01f;
	}

	//camRad = glm::length(camPos);

	//	����
	if (rotCamPosiCenter) {
		/*glm::vec3 ori = camPos - camDir;*/
		camDeg += 3.f;
		camPos.x = camRad * cos(glm::radians(camDeg));
		camPos.z = camRad * sin(glm::radians(camDeg));

		camDir = glm::normalize(camPos - glm::vec3(0.f, 0.f, 0.f));
		camU = glm::normalize(glm::cross(camDir, glm::vec3(0.f, 1.f, 0.f)));
		camV = glm::cross(-camDir, camU);
	}

	else if (rotCamNegaCenter) {
		camDeg -= 3.f;
		camPos.x = camRad * cos(glm::radians(camDeg));
		camPos.z = camRad * sin(glm::radians(camDeg));

		camDir = glm::normalize(camPos - glm::vec3(0.f, 0.f, 0.f));
		camU = glm::normalize(glm::cross(camDir, glm::vec3(0.f, 1.f, 0.f)));
		camV = glm::cross(-camDir, camU);
	}

	//	����
	if (rotCamPosiSelf) {
		glm::mat4 R = glm::rotate(glm::mat4(1.f), glm::radians(3.f), camV);
		camDir = glm::vec3(R * glm::vec4(camDir, 1.f));
		camU = glm::normalize(glm::cross(camDir, camV));
	}

	else if (rotCamNegaSelf) {
		glm::mat4 R = glm::rotate(glm::mat4(1.f), glm::radians(-30.f), camV);
		camDir = glm::vec3(R * glm::vec4(camDir, 1.f));
		camU = glm::normalize(glm::cross(camDir, camV));
	}

	if (rotateCamera == 1) {
		lightDeg += 5.f;
		lightPos.x = lightRad * cos(glm::radians(lightDeg));
		lightPos.z = lightRad * sin(glm::radians(lightDeg));

	}

	else if (rotateCamera == 2) {
		lightDeg -= 5.f;
		lightPos.x = lightRad * cos(glm::radians(lightDeg));
		lightPos.z = lightRad * sin(glm::radians(lightDeg));
	}

	//std::cout << "Length: " << camDeg2 << ", X: " << camPos.x << ", Z: " << camPos.z << '\n';
}

void Scene::draw()
{
	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 view;
	glm::mat4 proj;

	view = glm::lookAt(camPos, camPos - camDir, camV);

	if (Proj)
		proj = glm::perspective(glm::radians(45.f), 1.f, 0.1f, 50.f);

	else
		proj = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -100.f, 100.0f);

	spriteShader->setActive();
	spriteShader->setMatrixUniform("viewTransform", view);
	spriteShader->setMatrixUniform("projTransform", proj);
	spriteShader->setUniform3f("uCameraPos", camPos.x, camPos.y, camPos.z);
	spriteShader->setUniform3f("uLightPos", lightPos.x, lightPos.y, lightPos.z);
	spriteShader->setUniform1f("uAmbientLight", 0.1f);
	spriteShader->setUniform1f("uSpecularShininess", 64);
	spriteShader->setUniform1f("uSpecularStrength", 1.f);
	spriteShader->setUniform3f("uLightColor", lightColor.x, lightColor.y, lightColor.z);
	spriteShader->setUniform3f("uEmissiveColor", 0.f, 0.f, 0.f);

	plat->setActive(spriteShader);
	plat->DrawPlat(spriteShader->GetshaderProgram());

	crane->setActive(spriteShader);
	crane->Draw(spriteShader->GetshaderProgram());
}

void Scene::drawScene(int mode)
{	
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
		rotateCamera = 1;
		break;

	case 'Y':
		rotateCamera = 2;
		break;

	case 'c':
	case 'C':
		randomRGB();
		break;

	case 's':
	case 'S':
		rotateCamera = 0;
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


	/*case 's':
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
		break;*/


	/*case 'c':
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
		break;*/
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
	// ȭ�� ������Ʈ�� �ȴ�....

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

			std::cout << "��Ŭ�� : " << x << ", " << y << std::endl;
			std::cout << "OpenGL x ��ǥ�� " << mx << std::endl;
			std::cout << "OpenGL y ��ǥ�� " << my << std::endl;
			break;

		case GLUT_MIDDLE_BUTTON:
			std::cout << "��Ŭ�� : " << x << ", " << y << std::endl;
			break;

		case GLUT_RIGHT_BUTTON:
			std::cout << "��Ŭ�� : " << x << ", " << y << std::endl;
			break;

		case WHEEL_UP:
			std::cout << "��  �� : " << x << ", " << y << std::endl;
			break;

		case WHEEL_DOWN:
			std::cout << "�ٴٿ� : " << x << ", " << y << std::endl;
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
	std::uniform_int_distribution<int> uid{ 0,2 };

	switch (uid(rd)) {
	case 0:
		lightColor = glm::vec3(1.f);
		break;

	case 1:
		lightColor = glm::vec3(0.8f, 0.8f, 0.f);
		break;

	case 2:
		lightColor = glm::vec3(0.f, 0.8f, 0.8f);
		break;
	}
}

bool Scene::loadShaders()
{
	spriteShader = new Shader();

	if (!spriteShader->Load("C:\\Users\\worl\\Desktop\\Lecture\\2-2\\CG\\CG\\vertex3.vert", "C:\\Users\\worl\\Desktop\\Lecture\\2-2\\CG\\CG\\fragment2.frag"))
		return false;

	spriteShader->setActive();

	return true;
}