#include <iostream>
#include <fstream>
#include <chrono>
#include "Scene.h"

Scene::Scene(int winWidth, int winHeight)
	: width{ winWidth }, height{ winHeight }
{
	r = g = b = 0.0f;
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

	rotateLightPosi = rotateLightNega = false;

	star = new Star(0.f, 0.f);
	star->initVerts(0.6f, Position(1.f, 0.f, 0.f));

	planet = new Planet[3]{ Planet(1.3f, 0.0f), Planet(1.3f, 45.0f), Planet(1.3f, -45.0f) };

	planet[0].initVerts(0.3f, Position(0.f, 1.f, 0.f));
	planet[0].initOrbitVerts(star->getPos());
	planet[1].initVerts(0.3f, Position(0.f, 1.f, 0.f));
	planet[1].initOrbitVerts(star->getPos()); 
	planet[2].initVerts(0.3f, Position(0.f, 1.f, 0.f));
	planet[2].initOrbitVerts(star->getPos());

	lightColor = glm::vec3(1.f);
	lightDeg = 90.f;
	lightRad = glm::length(1.f);
	lightPos.x = lightRad * cos(glm::radians(lightDeg));
	lightPos.y = 0.f;
	lightPos.z = lightRad * sin(glm::radians(lightDeg));

	return true;
}

void Scene::update()
{
	if (rotateLightPosi) {
		lightDeg += 5.f;
		lightPos.x = lightRad * cos(glm::radians(lightDeg));
		lightPos.z = lightRad * sin(glm::radians(lightDeg));
		//star->setPos(lightPos.x, lightPos.y, lightPos.z);
		std::cout << "X " << lightPos.x << " Z " << lightPos.z << '\n';
	}

	else if (rotateLightNega) {
		lightDeg -= 5.f;
		lightPos.x = lightRad * cos(glm::radians(lightDeg));
		lightPos.z = lightRad * sin(glm::radians(lightDeg));
		//star->setPos(lightPos.x, lightPos.y, lightPos.z);
	}

	star->Update(Position(0.f, 0.f, 0.f));

	planet[0].Update(star->getPos());
	planet[1].Update(star->getPos());
	planet[2].Update(star->getPos());
}

void Scene::draw()
{
	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::vec3 camPos = glm::vec3(0.f, 0.f, 4.f);
	glm::vec3 camAt = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 camUp = glm::vec3(0.f, 1.f, 0.f);

	glm::mat4 view = glm::lookAt(camPos, camAt, camUp);
	glm::mat4 proj;

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

	star->Draw(spriteShader->GetshaderProgram(), star->getPos());

	//planet->setActive(spriteShader);
	planet[0].Draw(spriteShader->GetshaderProgram(), star->getPos());
	planet[1].Draw(spriteShader->GetshaderProgram(), star->getPos());
	planet[2].Draw(spriteShader->GetshaderProgram(), star->getPos());
}

void Scene::keyboard(unsigned char key)
{
	switch (key) {
	case 'c':
		randomRGB();
		break;

	case 'r':
		rotateLightPosi = !rotateLightPosi;
		rotateLightNega = false;
		break;

	case 'R':
		rotateLightPosi = false;
		rotateLightNega = !rotateLightNega;
		break;

	case 'h':
		hsr = !hsr; 
		
		if (hsr)
			glEnable(GL_DEPTH_TEST);

		else
			glDisable(GL_DEPTH_TEST);

		break;

	case 'y':
	case 'Y':
		planet[0].setRotateY();
		planet[1].setRotateY();
		planet[2].setRotateY();
		break;

	case 'z':
	case 'Z':
		planet[0].setRotateZ();
		planet[1].setRotateZ();
		planet[2].setRotateZ();
		break;

	case 'w':
		star->setMoveY(1);
		planet[0].setMoveY(1);
		planet[1].setMoveY(1);
		planet[2].setMoveY(1);
		break;

	case 'a':
		star->setMoveX(-1);
		planet[0].setMoveX(-1);
		planet[1].setMoveX(-1);
		planet[2].setMoveX(-1);
		break;

	case 's':
		star->setMoveY(-1);
		planet[0].setMoveY(-1);
		planet[1].setMoveY(-1);
		planet[2].setMoveY(-1);
		break;

	case 'd':
		star->setMoveX(1);
		planet[0].setMoveX(1);
		planet[1].setMoveX(1);
		planet[2].setMoveX(1);
		break;

	case '+':
		star->setMoveZ(1);
		planet[0].setMoveZ(1);
		planet[1].setMoveZ(1);
		planet[2].setMoveZ(1);
		break;

	case '-':
		star->setMoveZ(-1);
		planet[0].setMoveZ(-1);
		planet[1].setMoveZ(-1);
		planet[2].setMoveZ(-1);
		break;

	case 'p':
		Proj = !Proj;
		break;

	case 'm':
	case 'M':
		polygonMode = !polygonMode;

		if(polygonMode)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
}

void Scene::keyboardUp(unsigned char key)
{
	if (key == 'a' or key == 'd') {
		std::cout << "Scene::keyboardUp(a d)" << '\n';
		star->setMoveX(0);
		planet[0].setMoveX(0);
		planet[1].setMoveX(0);
		planet[2].setMoveX(0);
	}

	else if (key == 'w' or key == 's') {
		std::cout << "Scene::keyboardUp(w s)" << '\n';
		star->setMoveY(0);
		planet[0].setMoveY(0);
		planet[1].setMoveY(0);
		planet[2].setMoveY(0);
	}

	else if (key == '+' or key == '-') {
		std::cout << "Scene::keyboardUp(+ -)" << '\n';
		star->setMoveZ(0);
		planet[0].setMoveZ(0);
		planet[1].setMoveZ(0);
		planet[2].setMoveZ(0);
	}
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