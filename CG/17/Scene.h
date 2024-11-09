#pragma once
#include <gl/glew.h> // �ʿ��� ������� include
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

private:
	int width;
	int height;

	GLclampf r, g, b;

	glm::vec3 camPos;

	bool rotCam;
	float camDeg;
	float camRad;

	bool hsr;		//	���� ����
	bool Proj;		//	0: ��������, 1: ��������
	int shapeMode;	//	1: ����ü, 2: �簢��
	
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