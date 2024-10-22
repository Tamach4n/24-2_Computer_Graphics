#pragma once
#include <gl/glew.h> // �ʿ��� ������� include
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
	//GLuint sceneShader;

	int width;
	int height;

	GLclampf r, g, b;

	int drawMode;		//	��, ��	

	//class Vertex* spriteVertex;
	class Shader* spriteShader;

	std::vector<Shapes>shapesVec;


	void randomRGB();

	void initBuffer();
	void createSpriteVerts();
	bool loadShaders();

	std::random_device rd;
};

