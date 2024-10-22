#pragma once
#include <gl/glew.h> // 필요한 헤더파일 include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <string>
#include <vector>
#include <random>
#include "Vertex.h"	
#include "Shader.h"
#include "Shapes.h"

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
	//GLuint sceneShader;

	int width;
	int height;

	GLclampf r, g, b;

	int drawMode;		//	점, 선	

	//class Vertex* spriteVertex;
	class Shader* spriteShader;

	std::vector<Shapes>shapesVec;


	void randomRGB();

	void initBuffer();
	void createSpriteVerts();
	bool loadShaders();

	std::random_device rd;
};

