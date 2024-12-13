#include <iostream>
#include <fstream>
#include <chrono>
#include "Scene.h"
#include "Cube.h"
#include "Pyramid.h"

Scene::Scene(int winWidth, int winHeight)
	: width{ winWidth }, height{ winHeight }
{
	hsr = true;
	drawMode = true;
	r = g = b = 0.0f;
}

bool Scene::initialize()
{
	if (!loadShaders()) {
		std::cerr << "Failed to Load Shaders." << '\n';
		return false;
	}

	background = new Cube(this);
	background->Init();
	background->setScale(30.f);
	cube = new Cube(this);
	cube->Init();
	pyramid = new Pyramid(this);
	pyramid->Init();
	curr = cube;

	rotateCameraLeft = rotateCameraRight = rotateCameraUp = rotateCameraDown = false;

	isTurnedOn = true;
	rotateY = rotateX = false;
	orbitLight = transformLightRad = false;
	lightPos = glm::vec3(0.f, 0.f, 2.f);
	lightDeg = 90.f;
	lightRot = 0.f;
	dLightRad = 0.f;
	lightRad = 1.85f;
	orbitScale = 1.f;


	//lightPos.x = lightRad * cos(glm::radians(lightDeg));
	//lightPos.z = lightRad * sin(glm::radians(lightDeg));

	glm::vec3 camAt = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 camUp = glm::vec3(0.f, 1.f, 0.f);

	camPos = glm::vec3(0.f, 0.f, 3.f);
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

	if (rotateCameraDown || rotateCameraLeft || rotateCameraRight || rotateCameraUp) {
		static float pitch = 0.0f; // 상하 회전 각도
		const float pitchLimit = glm::radians(89.0f); // ±89도 제한

		if (rotateCameraLeft) {
			glm::mat4 R = glm::rotate(glm::mat4(1.f), glm::radians(3.f), camV);
			camDir = glm::normalize(glm::vec3(R * glm::vec4(camDir, 0.f)));
			camU = glm::normalize(glm::cross(camV, camDir));
		}

		if (rotateCameraRight) {
			glm::mat4 R = glm::rotate(glm::mat4(1.f), glm::radians(-3.f), camV);
			camDir = glm::normalize(glm::vec3(R * glm::vec4(camDir, 0.f)));
			camU = glm::normalize(glm::cross(camV, camDir));
		}

		if (rotateCameraUp) {
			pitch += glm::radians(-3.f);
			pitch = glm::clamp(pitch, -pitchLimit, pitchLimit); // 상하 각도 제한

			glm::mat4 R = glm::rotate(glm::mat4(1.f), glm::radians(-3.f), camU);
			glm::vec3 newDir = glm::normalize(glm::vec3(R * glm::vec4(camDir, 0.f)));

			// 상단 벡터 유지
			if (glm::abs(glm::dot(newDir, camV)) < 0.99f) { // 극점 방지
				camDir = newDir;
				camV = glm::normalize(glm::cross(camU, camDir));
			}
		}

		if (rotateCameraDown) {
			pitch += glm::radians(3.f);
			pitch = glm::clamp(pitch, -pitchLimit, pitchLimit); // 상하 각도 제한

			glm::mat4 R = glm::rotate(glm::mat4(1.f), glm::radians(3.f), camU);
			glm::vec3 newDir = glm::normalize(glm::vec3(R * glm::vec4(camDir, 0.f)));

			// 상단 벡터 유지
			if (glm::abs(glm::dot(newDir, camV)) < 0.99f) { // 극점 방지
				camDir = newDir;
				camV = glm::normalize(glm::cross(camU, camDir));
			}
		}
	}

	if (transformLightRad) {
		if (lightRad < 0.5f) {
			std::cout << "ToMaRe\n";
			transformLightRad = false;
			dLightRad = 0.f;
			lightRad += 0.01f;
		}

		orbitScale += dLightRad / 2;
		lightRad += dLightRad;
		lightPos.x = lightRad * cos(glm::radians(lightDeg));
		lightPos.z = lightRad * sin(glm::radians(lightDeg));
	}

	if (orbitLight) {
		lightDeg += lightRot;
		lightPos.x = lightRad * cos(glm::radians(lightDeg));
		lightPos.z = lightRad * sin(glm::radians(lightDeg));
		//std::cout << lightPos.x << " " << lightPos.z << '\n';
	}

	else if (rotateY || rotateX) {
		curr->Update();
	}
};

void Scene::draw()
{
	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 view = glm::lookAt(camPos, camPos - camDir, camV);
	glm::mat4 proj = glm::perspective(glm::radians(45.f), 1.f, 0.1f, 50.f);
	glm::mat4 mat(1.f);


	meshShader->setActive();
	meshShader->setMatrixUniform("viewTransform", view);
	meshShader->setMatrixUniform("projTransform", proj);
	meshShader->setUniform3f("uCameraPos", camPos.x, camPos.y, camPos.z);
	meshShader->setUniform3f("uLightPos", lightPos.x, lightPos.y, lightPos.z);
	meshShader->setUniform1f("uAmbientLight", 0.3f);
	meshShader->setUniform1f("uSpecularShininess", 64);
	meshShader->setUniform1f("uSpecularStrength", 1.f);

	if (isTurnedOn)
		meshShader->setUniform3f("uLightColor", 1.f, 1.f, 1.f);

	else
		meshShader->setUniform3f("uLightColor", 0.f, 0.f, 0.f);

	spriteShader->setUniform3f("uEmissiveColor", 0.f, 0.f, 0.f);
	curr->Draw(meshShader);
	spriteShader->setUniform3f("uEmissiveColor", 0.2f, 0.2f, 0.2f);
	background->DrawBG(meshShader);
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
	{
		glm::vec3 v = curr->getRotation();

		if (rotateX && v.x > 0) {
			std::cout << "Rotation Positive Off\n";
			rotateX = false;
			curr->setRotation(0.f, 0.f, 0.f);
		}

		else {
			std::cout << "Rotation Positive On\n";
			rotateX = true;
			curr->setRotation(3.f, 0.f, 0.f);;
		}

		break;
	}

	case 'X':
	{
		glm::vec3 v = curr->getRotation();

		if (rotateX && v.x > 0) {
			std::cout << "Rotation Positive Off\n";
			rotateX = false;
			curr->setRotation(0.f, 0.f, 0.f);
		}

		else {
			std::cout << "Rotation Positive On\n";
			rotateX = true;
			curr->setRotation(-3.f, 0.f, 0.f);;
		}

		break;
	}

	case 'y':
	{
		glm::vec3 v = curr->getRotation();

		if (rotateY && v.y > 0) {
			std::cout << "Rotation Positive Off\n";
			rotateY = false;
			curr->setRotation(0.f, 0.f, 0.f);
		}

		else {
			std::cout << "Rotation Positive On\n";
			rotateY = true;
			curr->setRotation(0.f, 3.f, 0.f);;
		}

		break;
	}

	case 'Y':
	{
		glm::vec3 v = curr->getRotation();

		if (rotateY && v.y < 0) {
			std::cout << "Rotation Negative Off\n";
			rotateY = false;
			curr->setRotation(0.f, 0.f, 0.f);
		}

		else {
			std::cout << "Rotation Negative On\n";
			rotateY = true;
			curr->setRotation(0.f, -3.f, 0.f);
		}

		break;
	}

	case 'r':
		if (orbitLight&& lightRot > 0) {
			std::cout << "Uchimawari Off\n";
			orbitLight = false;
			lightRot = 0.f;
		}

		else {
			std::cout << "Uchimawari On\n";
			orbitLight = true;
			lightRot = 3.f;
		}

		break;

	case 'R':
		if (orbitLight && lightRot < 0) {
			std::cout << "Sotomawari Off\n";
			orbitLight = false;
			lightRot = 0.f;
		}

		else {
			std::cout << "Sotomawari On\n";
			orbitLight = true;
			lightRot = -3.f;
		}

		break;

	case 'z':
		if (transformLightRad && dLightRad < 0) {
			std::cout << "ToMaRe\n";
			transformLightRad = false;
			dLightRad = 0.f;
		}

		else {
			std::cout << "Chikazuku\n";
			transformLightRad = true;
			dLightRad = -0.01f;
		}

		break;

	case 'Z':
		if (transformLightRad && dLightRad > 0) {
			std::cout << "ToMaRe\n";
			transformLightRad = false;
			dLightRad = 0.f;
		}

		else {
			std::cout << "OMG Go Outside\n";
			transformLightRad = true;
			dLightRad = 0.01f;
		}

		break;

	case 'm':
	case 'M':
		isTurnedOn = !isTurnedOn;

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
		rotateCameraUp = true;
		break;

	case GLUT_KEY_DOWN:
		rotateCameraDown = true;
		break;

	case GLUT_KEY_LEFT:
		rotateCameraRight = true;
		break;

	case GLUT_KEY_RIGHT:
		rotateCameraLeft = true;
		break;
	}
}

void Scene::specialKeyboardUp(int key)
{
	switch (key) {
	case GLUT_KEY_UP:
		rotateCameraUp = false;
		break;

	case GLUT_KEY_DOWN:
		rotateCameraDown = false;
		break;

	case GLUT_KEY_LEFT:
		rotateCameraRight = false;
		break;

	case GLUT_KEY_RIGHT:
		rotateCameraLeft = false;
		break;
	}
}

void Scene::mouse(int button, int state, int x, int y)
{
	// 화면 업데이트가 된다....

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

			std::cout << "좌클릭 : " << x << ", " << y << std::endl;
			std::cout << "OpenGL x 좌표는 " << mx << std::endl;
			std::cout << "OpenGL y 좌표는 " << my << std::endl;
			break;

		case GLUT_MIDDLE_BUTTON:
			std::cout << "휠클릭 : " << x << ", " << y << std::endl;
			break;

		case GLUT_RIGHT_BUTTON:
			std::cout << "우클릭 : " << x << ", " << y << std::endl;
			break;

		case WHEEL_UP:
			std::cout << "휠  업 : " << x << ", " << y << std::endl;
			break;

		case WHEEL_DOWN:
			std::cout << "휠다운 : " << x << ", " << y << std::endl;
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

Texture* Scene::getTexture(const std::string& fileName)
{
	Texture* tex = nullptr;

	auto iter = mTextures.find(fileName);

	if (iter != mTextures.end())
		tex = iter->second;

	else {
		tex = new Texture;

		if (tex->load(fileName))
			mTextures.emplace(fileName, tex);

		else {
			delete tex;
			tex = nullptr;
		}
	}

	return tex;
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
	meshShader = new Shader();

	if (!meshShader->Load("C:\\Users\\worl\\Desktop\\Lecture\\2-2\\CG\\CG\\Phong.vert", "C:\\Users\\worl\\Desktop\\Lecture\\2-2\\CG\\CG\\Phong.frag"))
		return false;

	spriteShader = new Shader();

	if (!spriteShader->Load("C:\\Users\\worl\\Desktop\\Lecture\\2-2\\CG\\CG\\Phong.vert", "C:\\Users\\worl\\Desktop\\Lecture\\2-2\\CG\\CG\\Phong.frag"))
		return false;

	return true;
}