#include <iostream>
#include <fstream>
#include <chrono>
#include "Scene.h"

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

	createSpriteVerts();

	
	randomRGB();
	
	return true;
}

void Scene::update()
{
	if (curSpiral >= spiralVec.size())
		return; 
	
	static bool startDraw = true;

	if (startDraw) {
		randomRGB();
		startDraw = false;
	}

	if (!spiralVec[curSpiral].update(selectMode)) {
		++curSpiral;
		startDraw = true;
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
		curSpiral = 0;
		setSpiralVec(1);
		break;

	case '2':
		curSpiral = 0;
		setSpiralVec(2);
		break;

	case'3':
		curSpiral = 0;
		setSpiralVec(3);
		break;

	case '4':
		curSpiral = 0;
		setSpiralVec(4);
		break;

	case '5':
		curSpiral = 0;
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
	float vertices[] = {
		0.0f, 0.2f, 0.f,	0.f, 1.f, 1.f,
		-0.1f, 0.f, 0.f,	0.f, 1.f, 1.f,
		0.1f, 0.0f, 0.f,	0.f, 1.f, 1.f
	};

	unsigned int indices[] = {
		0, 1, 2
	};

	spriteVertex = new Vertex(vertices, 3, indices, 3);

}
//	---------------------------------

bool Scene::loadShaders()
{
	spriteShader = new Shader();

	if (!spriteShader->Load("C:\\Users\\worl\\Desktop\\Lecture\\2-2\\CG\\CG\\10\\vertex.glsl", "C:\\Users\\worl\\Desktop\\Lecture\\2-2\\CG\\CG\\10\\fragment.glsl"))
		return false;

	spriteShader->setActive();
}