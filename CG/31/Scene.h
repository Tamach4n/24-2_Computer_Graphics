#pragma once
#include <gl/glew.h> // �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include "glm/glm.hpp"
#include "glm/ext.hpp"

#include <unordered_map>
#include <string>
#include <vector>
#include <random>
#include "Vertex.h"	
#include "shader.h"
#include "Texture.h"

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

	class Texture* getTexture(const std::string& fileName);

private:
	std::unordered_map<std::string, class Texture*> mTextures;

	int width;
	int height;

	glm::vec3 camPos;
	glm::vec3 camDir;
	glm::vec3 camU;
	glm::vec3 camV;

	GLclampf r, g, b;

	bool hsr;			//	���� ���� - Hidden Surface Removal
	bool drawMode;		//	0: ���̾�, 1: �ָ���

	class Shader* meshShader;
	class Shader* spriteShader;

	class Cube* background;
	class Shape* curr;
	class Cube* cube;
	class Pyramid* pyramid;

	bool isTurnedOn;
	bool rotateY;
	bool rotateX;
	bool orbitLight;
	bool transformLightRad;

	bool rotateCameraLeft;
	bool rotateCameraRight;
	bool rotateCameraUp;
	bool rotateCameraDown;

	glm::vec3 lightPos;	//	lightSource�� ����
	float lightDeg;
	float lightRot;
	float lightRad;
	float dLightRad;

	float orbitScale;

	void randomRGB();

	void initBuffer();

	bool loadShaders();

	std::random_device rd;
};