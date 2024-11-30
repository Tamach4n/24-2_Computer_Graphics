#include <iostream>
#include <fstream>
#include <chrono>
#include "Scene.h"

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

	cube = new Cube;
	cube->Init();
	pyramid = new Pyramid;
	pyramid->Init();
	curr = cube;

	isTurnedOn = true;
	rotateLight = orbitLight = transformLightRad = false;
	lightPos = glm::vec3(0.f, 0.f, 2.f);
	lightDeg = 90.f;
	lightRot = 0.f;
	dLightRad = 0.f;
	lightRad = 1.85f;
	orbitScale = 1.f;

	lightSource = new Cube();
	lightSource->Init();
	lightSource->setDegree(glm::vec3(0.f, 0.f, 0.f));
	lightSource->setPosition(lightPos);
	lightSource->setScale(0.1f);

	lightPos.x = lightRad * cos(glm::radians(lightDeg));
	lightPos.z = lightRad * sin(glm::radians(lightDeg));

	glm::vec3 camAt = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 camUp = glm::vec3(0.f, 1.f, 0.f);

	camPos = glm::vec3(-3.f, 3.f, 3.f);
	camDir = glm::normalize(camPos - camAt);
	camU = glm::normalize(glm::cross(camUp, camDir));
	camV = glm::cross(camDir, camU);
	
	createAxisVerts();
	createOrbitVerts();
	
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

	if (transformLightRad) {
		if (lightRad < 0.5f) {
			std::cout << "ToMaRe\n";
			transformLightRad = false;
			dLightRad = 0.f;
			lightSource->setDirection(0.f, 0.f, 0.f);
			lightRad += 0.01f;
		}

		orbitScale += dLightRad / 2;
		lightRad += dLightRad;
		lightPos.x = lightRad * cos(glm::radians(lightDeg));
		lightPos.z = lightRad * sin(glm::radians(lightDeg));
		lightSource->Update();
	}

	if (orbitLight) {
		lightSource->Update();
		lightDeg += lightRot;
		lightPos.x = lightRad * cos(glm::radians(lightDeg));
		lightPos.z = lightRad * sin(glm::radians(lightDeg));
		//std::cout << lightPos.x << " " << lightPos.z << '\n';
	}

	else if (rotateLight) {
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

	{
		axisVertex->setActive();
		axisShader->setActive();
		axisShader->setMatrixUniform("viewTransform", view);
		axisShader->setMatrixUniform("projTransform", proj);
		axisShader->setMatrixUniform("modelTransform", mat);
		glDrawArrays(GL_LINES, 0, 6);
	}
	{
		glm::mat4 s = glm::scale(mat, glm::vec3(orbitScale));
		orbitVertex->setActive();
		orbitShader->setActive();
		orbitShader->setMatrixUniform("viewTransform", view);
		orbitShader->setMatrixUniform("projTransform", proj);
		orbitShader->setMatrixUniform("modelTransform", s);
		glDrawArrays(GL_LINE_LOOP, 0, 360);
	}
	{
		spriteShader->setActive();
		spriteShader->setMatrixUniform("viewTransform", view);
		spriteShader->setMatrixUniform("projTransform", proj);
		spriteShader->setUniform3f("uCameraPos", camPos.x, camPos.y, camPos.z);
		spriteShader->setUniform3f("uLightPos", lightPos.x, lightPos.y, lightPos.z);
		spriteShader->setUniform1f("uAmbientLight", 0.1f);
		spriteShader->setUniform1f("uSpecularShininess", 64);
		spriteShader->setUniform1f("uSpecularStrength", 1.f);

		if (isTurnedOn)
			spriteShader->setUniform3f("uLightColor", 1.f, 1.f, 1.f);

		else
			spriteShader->setUniform3f("uLightColor", 0.f, 0.f, 0.f);

		spriteShader->setUniform3f("uEmissiveColor", 1.f, 1.f, 1.f);
		lightSource->Draw(spriteShader);
		spriteShader->setUniform3f("uEmissiveColor", 0.f, 0.f, 0.f);
		curr->Draw(spriteShader);
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

	case 'y':
	{
		glm::vec3 v = curr->getRotation();

		if (rotateLight && v.y > 0) {
			std::cout << "Rotation Positive Off\n";
			rotateLight = false;
			curr->setRotation(0.f, 0.f, 0.f);
		}

		else {
			std::cout << "Rotation Positive On\n";
			rotateLight = true;
			curr->setRotation(0.f, 3.f, 0.f);;
		}

		break;
	}

	case 'Y':
	{
		glm::vec3 v = curr->getRotation();

		if (rotateLight && v.y < 0) {
			std::cout << "Rotation Negative Off\n";
			rotateLight = false;
			curr->setRotation(0.f, 0.f, 0.f);
		}

		else {
			std::cout << "Rotation Negative On\n";
			rotateLight = true;
			curr->setRotation(0.f, -3.f, 0.f);
		}

		break;
	}

	case 'r':
		if (orbitLight&& lightRot > 0) {
			std::cout << "Uchimawari Off\n";
			orbitLight = false;
			lightRot = 0.f;
			lightSource->setRotation(0.f, 0.f, 0.f);
			lightSource->setOrbit(false);
		}

		else {
			std::cout << "Uchimawari On\n";
			orbitLight = true;
			lightRot = 3.f;
			lightSource->setRotation(0.f, -3.f, 0.f);
			lightSource->setOrbit(true);
		}

		break;

	case 'R':
		if (orbitLight && lightRot < 0) {
			std::cout << "Sotomawari Off\n";
			orbitLight = false;
			lightRot = 0.f;
			lightSource->setRotation(0.f, 0.f, 0.f);
			lightSource->setOrbit(false);
		}

		else {
			std::cout << "Sotomawari On\n";
			orbitLight = true;
			lightRot = -3.f;
			lightSource->setRotation(0.f, 3.f, 0.f);
			lightSource->setOrbit(true);
		}

		break;

	case 'z':
		if (transformLightRad && dLightRad < 0) {
			std::cout << "ToMaRe\n";
			transformLightRad = false;
			dLightRad = 0.f;
			lightSource->setDirection(0.f, 0.f, 0.f);
		}

		else {
			std::cout << "Chikazuku\n";
			transformLightRad = true;
			dLightRad = -0.01f;
			lightSource->setDirection(0.f, 0.f, -0.01f);
		}

		break;

	case 'Z':
		if (transformLightRad && dLightRad > 0) {
			std::cout << "ToMaRe\n";
			transformLightRad = false;
			dLightRad = 0.f;
			lightSource->setDirection(0.f, 0.f, 0.f);
		}

		else {
			std::cout << "OMG Go Outside\n";
			transformLightRad = true;
			dLightRad = 0.01f;
			lightSource->setDirection(0.f, 0.f, 0.01f);
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

void Scene::createOrbitVerts()
{
	float r = 2.f;
	std::vector<float> VAO;

	for (int i = 0; i < 360; ++i) {
		VAO.push_back(r * cos(glm::radians(static_cast<float>(i))));
		VAO.push_back(0.f);
		VAO.push_back(r * sin(glm::radians(static_cast<float>(i))));
		VAO.push_back(0.1f);
		VAO.push_back(0.1f);
		VAO.push_back(0.1f);
	}

	orbitVertex = new Vertex(VAO);
}

bool Scene::loadShaders()
{
	axisShader = new Shader();

	if (!axisShader->Load("C:\\Users\\worl\\Desktop\\Lecture\\2-2\\CG\\CG\\vertex2.vert", "C:\\Users\\worl\\Desktop\\Lecture\\2-2\\CG\\CG\\fragment.frag"))
		return false;

	orbitShader = new Shader();

	if (!orbitShader->Load("C:\\Users\\worl\\Desktop\\Lecture\\2-2\\CG\\CG\\vertex2.vert", "C:\\Users\\worl\\Desktop\\Lecture\\2-2\\CG\\CG\\fragment.frag"))
		return false;

	spriteShader = new Shader();

	if (!spriteShader->Load("C:\\Users\\worl\\Desktop\\Lecture\\2-2\\CG\\CG\\vertex3.vert", "C:\\Users\\worl\\Desktop\\Lecture\\2-2\\CG\\CG\\fragment2.frag"))
		return false;

	return true;
}