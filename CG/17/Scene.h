#pragma once
#include <glew.h> // �ʿ��� ������� include
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
	// ���� ��������, update, draw..., Ű �Է�
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

	bool hsr;		//	���� ����
	bool Proj;		//	0: ��������, 1: ��������
	int shapeMode;	//	1: ����ü, 2: �簢��
	
	class Shader* spriteShader;

	Shape* axisShape;
	Shape* shape;

	void randomRGB();

	bool loadShaders();

	std::random_device rd;
};