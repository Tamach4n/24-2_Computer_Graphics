#include <iostream>
#include <fstream>
#include <chrono>
#include "Scene.h"

#define _USE_MATH_DEFINES
#include <math.h>

Scene::Scene(int winWidth, int winHeight)
	: width{ winWidth }, height{ winHeight }
{
	selectMode = 1;
	//sizeSpirals = 0;
}

bool Scene::initialize()
{
	if (!loadShaders()) {
		std::cerr << "Failed to Load Shaders." << '\n';
		return false;
	}

	createSpriteVerts();

	
	randomRGB();
	
	return true;
}

void Scene::update()
{
	if (!spiralVec.empty()) {
		for (int i = 0; i < spiralVec.size(); ++i)
			spiralVec[i].update(verts.size() / 2);
	}
}

void Scene::draw()
{
	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	spriteShader->setActive();
	spriteVertex->setActive();

	for (int i = 0; i < spiralVec.size(); ++i)
		spiralVec[i].draw(spriteShader, selectMode);
}

void Scene::keyboard(unsigned char key)
{
	switch (key) {
	case 'p':
		std::cout << "Point mode" << '\n';
		selectMode = 0;
		break;

	case 'l':
		std::cout << "Line mode" << '\n';
		selectMode = 1;
		break;

	case'1':
		setSpiralVec(1);
		break;

	case '2':
		setSpiralVec(2);
		break;

	case'3':
		setSpiralVec(3);
		break;

	case '4':
		setSpiralVec(4);
		break;

	case '5':
		setSpiralVec(5);
		break;		
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

	switch (state) {
	case GLUT_DOWN:
		switch (button) {
		case GLUT_LEFT_BUTTON:
			std::cout << "ÁÂÅ¬¸¯ : " << x << ", " << y << std::endl;
			std::cout << "OpenGL x ÁÂÇ¥´Â " << (static_cast<float>(x) / width * 2.f - 1.f) << std::endl;
			std::cout << "OpenGL y ÁÂÇ¥´Â " << (static_cast<float>(height - y) / height * 2.f - 1.f) << std::endl;
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

void Scene::setSpiralVec(int size)
{
	if (!spiralVec.empty())
		spiralVec.clear();

	for (int i = 0; i < size; ++i) {
		spiralVec.push_back(Spiral());
	}
}

void Scene::randomRGB()
{
	std::uniform_real_distribution<float> urd{ 0.f, 1.f };

	r = urd(rd);
	g = urd(rd);
	b = urd(rd);
}

//	---------------------------------
void Scene::createSpriteVerts()
{
	//float vertices[] = {
	//	0.0f, 0.2f, 0.f,	0.f, 1.f, 1.f,
	//	-0.1f, 0.f, 0.f,	0.f, 1.f, 1.f,
	//	0.1f, 0.0f, 0.f,	0.f, 1.f, 1.f
	//};
	//
	//unsigned int indices[] = {
	//	0, 1, 2
	//};
	//
	//spriteVertex = new Vertex(vertices, 3, indices, 3);

	//-----------------------------------------------------

	{
		float angle = 0.f;
		float radius = 0.f;
		float angleSpeed = 0.1f;
		float radiusGrowth = 0.001f;
		float xPos = 0.126f;
		float newX = 0.f;
		float newY = 0.f;
	
		for (; angle >= 4 * M_PI; angle += angleSpeed) {
			angle += angleSpeed;
			radius += radiusGrowth;
	
			newX = radius * cos(angle) - xPos;
			newY = radius * sin(angle);
	
			verts.push_back(newX);
			verts.push_back(newY);
		}
	
		angle = -angle;
	
		for (; angle >= 0.f; angle += angleSpeed) {
			angle += angleSpeed;
			radius -= radiusGrowth;
	
			newX = radius * cos(angle) + xPos;
			newY = radius * sin(angle);
	
			verts.push_back(newX);
			verts.push_back(newY);
		}

		unsigned int inddices[] = { 0, 1 };
	
		angleSpeed = 0.f;
		radius = 0.f;

		spriteVertex = new Vertex(verts, inddices, 2);
	}

	//GLuint VBO, VAO;
	//glGenVertexArrays(1, &VAO);
	//glGenBuffers(1, &VBO);
	//
	//glBindVertexArray(VAO);
	//
	//// Load the vertex data into the buffer
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
	//
	//// Specify the layout of the vertex data (x, y positions)
	//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);

}
//	---------------------------------

bool Scene::loadShaders()
{
	spriteShader = new Shader();

	if (!spriteShader->Load("C:\\Users\\worl\\Desktop\\Lecture\\2-2\\CG\\CG\\10\\vertex.glsl", "C:\\Users\\worl\\Desktop\\Lecture\\2-2\\CG\\CG\\10\\fragment.glsl"))
		return false;

	spriteShader->setActive();
}