#pragma once
#include <gl/glew.h> // 필요한 헤더파일 include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <string>
#include <list>
#include "Quadrant.h"

class Scene
{
private:
	GLuint triangleShader;

	int width;
	int height;

	int drawMode;
	Quadrant* quadrant;

public:
	// 게임 로직에서, update, draw..., 키 입력
	Scene(int winWidth, int winHeight);

	void initialize();

	void update();

	void draw();

	void keyboard(unsigned char key);		// press
	void keyboardUp(unsigned char key);		// release
	void specialKeyboard(int key);			// press
	void specialKeyboardUp(int key);		// release
	void mouse(int button, int state, int x, int y);

	void setWindowSize(int winWidth, int winHeight);
	//int getMode();

private:
	GLuint makeShader(std::string vertexFilename, std::string fragmentFilename);
	std::string readFile(std::string filename);
};