#include <iostream>
#include <fstream>
#include <chrono>
#include "Scene.h"

Scene::Scene(int winWidth, int winHeight)
	: width{ winWidth }, height{ winHeight }
{
	drawMode = 0;
	doubleFace = false;
	startIndex = 0;
	faceNum1 = faceNum2 = 0;
	indexCount = 36;
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
	tetra = new Tetrahedron;
	tetra->Init();

	//randomRGB();
	
	return true;
}

void Scene::update()
{
	//std::cout << "Scene::update" << '\n';
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

		if (drawMode == 0) {
			cube->setActive();

			if (!doubleFace)
				cube->Draw(startIndex, indexCount);

			else {
				cube->Draw(faceNum1 * 6, indexCount);
				cube->Draw(faceNum2 * 6, indexCount);
			}
		}

		else {
			tetra->setActive();

			if (!doubleFace)
				tetra->Draw(startIndex, indexCount);

			else {
				tetra->Draw(faceNum1 * 3, indexCount);
				tetra->Draw(faceNum2 * 3, indexCount);
			}
		}
	}
}

void Scene::keyboard(unsigned char key)
{
	if ((key >= '1' && key <= '6') || key == 'c') {
		doubleFace = false;
		drawMode = 0;
		indexCount = 6;

		switch (key) {
		case '1':
			startIndex = 0;
			break;

		case '2':
			startIndex = 6;
			break;

		case '3':
			startIndex = 12;
			break;

		case '4':
			startIndex = 18;
			break;

		case '5':
			startIndex = 24;
			break;

		case '6':
			startIndex = 30;
			break;

		case 'c':	
		case 'C':
			doubleFace = true;
			randomFace();
			break;
		}
	}

	else if ((key >= '7' && key <= '9') || key == '0' || key == 't') {
		doubleFace = false;
		drawMode = 1;
		indexCount = 3;

		switch (key) {
		case '7':
			startIndex = 0;
			break;

		case '8':
			startIndex = 3;
			break;

		case '9':
			startIndex = 6;
			break;

		case '0':
			startIndex = 9;
			break;

		case 't':	
		case 'T':
			doubleFace = true;
			randomFace();
			break;
		}
	}	
}

void Scene::keyboardUp(unsigned char key)
{
}

void Scene::specialKeyboard(int key)
{
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

void Scene::randomFace()
{
	std::uniform_int_distribution<> uid(0, 5);

	faceNum1 = uid(rd);

	while (true) {
		faceNum2 = uid(rd);

		if (faceNum1 != faceNum2)	break;
	}
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
		0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 1.0f
	};

	axisVertex = new Vertex(VAO, 4);
}

bool Scene::loadShaders()
{
	spriteShader = new Shader();

	if (!spriteShader->Load("C:\\Users\\worl\\Desktop\\Lecture\\2-2\\CG\\CG\\13\\vertex.vert", "C:\\Users\\worl\\Desktop\\Lecture\\2-2\\CG\\CG\\13\\fragment.frag"))
		return false;

	spriteShader->setActive();

	return true;
}