#pragma once
#include <glew.h> // 필요한 헤더파일 include
#include <freeglut.h>
#include <freeglut_ext.h>
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

	void removeShape();

private:
	int width;
	int height;

	GLclampf r, g, b;

	bool hsr;		//	은면 제거
	bool Proj;		//	0: 직각투영, 1: 원근투영
	int shapeMode;	//	1: 육면체, 2: 사각뿔
	
	class Shader* spriteShader;

	Shape* axisShape;
	Shape* shape;

	void randomRGB();

	bool loadShaders();

	std::random_device rd;
};