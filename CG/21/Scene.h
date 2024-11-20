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

class Scene
{
public:
	// 게임 로직에서, update, draw..., 키 입력
	Scene(int winWidth, int winHeight);

	bool initialize();

	void update();

	void draw();
	void drawScene(int mode);

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

	glm::vec3 camPos[3];
	glm::vec3 camDir[3];
	glm::vec3 camU[3];
	glm::vec3 camV[3];

	bool rotCamPosiSelf;
	bool rotCamNegaSelf;
	bool rotCamPosiCenter;
	bool rotCamNegaCenter;
	bool movCamPosiX;
	bool movCamNegaX;
	bool movCamPosiZ;
	bool movCamNegaZ;

	float camDeg;
	float camRad;

	bool hsr;		//	은면 제거
	bool Proj;		//	0: 직각투영, 1: 원근투영
	bool polygonMode;	//	0: 선, 1: 면
	
	class Shader* spriteShader;

	class Shape* crane;
	class Shape* plat;

	void randomRGB();

	bool loadShaders();

	std::random_device rd;
};