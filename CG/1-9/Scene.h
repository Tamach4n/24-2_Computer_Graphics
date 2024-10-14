#pragma once
#include <gl/glew.h> // �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include "MoveTriangle.h"
#include <string>

class Scene
{
private:
	GLuint sceneShader;

	int width;
	int height;

	int selectMode;

	MoveTriangle triangles[4];


	GLuint VAO[4];	// �ﰢ���� ���⿡ ���� 4������ �ﰢ��

public:
	// ���� ��������, update, draw..., Ű �Է�
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

private:
	void initBuffer();

	static GLuint makeShader(std::string vertexFilename, std::string fragmentFilename);
	static std::string readFile(std::string filename);

};

