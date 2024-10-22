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
#include "Tetrahedron.h"

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

	void randomFace();

private:
	int width;
	int height;

	GLclampf r, g, b;

	int drawMode;		//	0: 육면체, 1: 사면체
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