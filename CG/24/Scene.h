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

	glm::vec3 camPos;
	glm::vec3 camDir;
	glm::vec3 camU;
	glm::vec3 camV;

	GLclampf r, g, b;

	bool hsr;			//	은면 제거 - Hidden Surface Removal
	bool drawMode;		//	0: 와이어, 1: 솔리드

	class Vertex* axisVertex;
	class Shader* axisShader;
	class Shader* spriteShader;
	class Vertex* orbitVertex;
	class Shader* orbitShader;

	class Shape* curr;
	class Shape* cube;
	class Shape* pyramid;
	class Shape* lightSource;

	bool isTurnedOn;
	bool rotateY;
	bool orbitLight;
	bool transformLightRad;
	glm::vec3 lightPos;	//	lightSource의 중점
	float lightDeg;
	float lightRot;
	float lightRad;
	float dLightRad;

	float orbitScale;

	void randomRGB();

	void initBuffer();

	void createAxisVerts();
	void createOrbitVerts();

	bool loadShaders();

	std::random_device rd;
};