#pragma once
#include <gl/glew.h> // 필요한 헤더파일 include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include "glm/glm.hpp"
#include "glm/ext.hpp"

#include <string>
#include <vector>
#include <random>
#include "Vertex.h"	
#include "shader.h"
#include "Shape.h"
#include "Cube.h"
#include "Pyramid.h"

class Scene
{
public:
	// 게임 로직에서, update, draw..., 키 입력
	Scene(int winWidth, int winHeight);

	bool initialize();

	void update();

	void draw();

	void keyboard(unsigned char key);		// press
	void keyboardUp(unsigned char key);		// release
	void specialKeyboard(int key);			// press
	void specialKeyboardUp(int key);		// release
	void mouse(int button, int state, int x, int y);

	void setWindowSize(int winWidth, int winHeight);

private:
	int width;
	int height;

	GLclampf r, g, b;

	glm::vec3 camPos;

	bool rotCam;
	float camDeg;
	float camRad;

	bool hsr;		//	은면 제거
	bool Proj;		//	0: 직각투영, 1: 원근투영
	int shapeMode;	//	1: 육면체, 2: 사각뿔
	
	class Shader* spriteShader;

	Shape* axisShape;
	Shape* shape;

	void randomRGB();

	bool loadShaders();

	template <typename T>
	void changeShape() {
		Shape* temp = new Shape(*shape);
		delete shape;
		shape = new T();
		*shape = *temp;
		delete temp;
		shape->initVerts();
	}

	std::random_device rd;
};