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
#include "Tetrahedron.h"

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

	void randomFace();

private:
	int width;
	int height;

	GLclampf r, g, b;

	int drawMode;		//	0: ����ü, 1: ���ü
	bool doubleFace;

	int startIndex, indexCount;

	int faceNum1, faceNum2;

	class Vertex* axisVertex;
	class Shader* spriteShader;

	class Cube* cube;
	class Tetrahedron* tetra;

	void randomRGB();

	void initBuffer();

	void createAxisVerts();
	//void createSpriteVerts();

	bool loadShaders();

	std::random_device rd;
};