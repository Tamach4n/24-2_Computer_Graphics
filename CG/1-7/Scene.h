#pragma once
#include <gl/glew.h> // �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <string>
#include <list>


class Scene
{
private:
	GLuint pointShader;
	GLuint triangleShader;
	GLuint rectangleShader;

	int width;
	int height;

	std::pair<float, float>* select[11];
	std::list<std::pair<float, float>> pointList;
	std::list<std::pair<float, float>> lineList;
	std::list<std::pair<float, float>> triangleList;
	std::list<std::pair<float, float>> rectangleList;

	int selectPolygon;	// 0 : ��, 1 : �ﰢ��, 2 : �簢��
	int end;

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

	int getEnd();
	int selectShape();
	void moveShape(int dx, int dy);

private:
	GLuint makeShader(std::string vertexFilename, std::string fragmentFilename);
	std::string readFile(std::string filename);
};

