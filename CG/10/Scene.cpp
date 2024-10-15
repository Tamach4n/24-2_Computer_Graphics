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
	curSpiral = 0;
}

bool Scene::initialize()
{
	if (!loadShaders()) {
		std::cerr << "Failed to Load Shaders." << '\n';
		return false;
	}

	spiralVec.clear();
	verts.clear();
	
	createSpriteVerts();

	randomRGB();
	
	return true;
}

void Scene::update()
{
	if (spiralVec.empty())
		return;

	std::cout << "Scene::update" << '\n';
	
	int count = verts.size() / 2;

	for (int i = 0; i < spiralVec.size(); ++i) {
		spiralVec[i].update(count);
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

			curSpiral = 0;
			spiralVec.clear();
			spiralVec.push_back(Spiral(mx, my));

			delete spriteVertex;
			createSpriteVerts();

			randomRGB();
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
	curSpiral = 0; 

	randomRGB();

	delete spriteVertex;
	createSpriteVerts();

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
	float angle = 0.f;
	float radius = 0.f;
	float angleSpeed = 0.3f;
	float radiusGrowth = 0.003f;
	float xPos = 0.f;
	float newX = 0.f;
	float newY = 0.f;
	int count = 0;
	std::uniform_int_distribution<> uid(0, 1);
	int dir = uid(rd) * 2 - 1;

	verts.clear();

	while (angle <= 4 * M_PI) {
		newX = dir * (radius * cos(angle));
		newY = radius * sin(angle);
		std::cout << "Angle: " << angle << " newX: " << newX << " newY: " << newY << '\n';
		verts.push_back(newX);
		verts.push_back(newY);
		
		angle += angleSpeed;
		radius += radiusGrowth;
	}

	angle = -angle;
	xPos += (dir == 1) ? 2 * radius : -2 * radius;

	while (angle <= 0.f) {
		newX = -dir * (radius * cos(angle)) + xPos;
		newY = radius * sin(angle);
		std::cout << "Angle: " << angle << " newX: " << newX << " newY: " << newY << '\n';
		verts.push_back(newX);
		verts.push_back(newY);
		
		angle += angleSpeed;
		radius -= radiusGrowth;
	}
	std::cout << verts.size() / 2 << '\n';
	spriteVertex = new Vertex(verts);
}
//	---------------------------------

bool Scene::loadShaders()
{
	spriteShader = new Shader();

	if (!spriteShader->Load("C:\\Users\\worl\\Desktop\\Lecture\\2-2\\CG\\CG\\10\\vertex.glsl", "C:\\Users\\worl\\Desktop\\Lecture\\2-2\\CG\\CG\\10\\fragment.glsl"))
		return false;

	spriteShader->setActive();

	return true;
}