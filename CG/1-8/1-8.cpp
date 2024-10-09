#include <iostream>
#include <fstream>
#include <random>
#include "1-8.h"

//std::random_device rd;
//std::default_random_engine dre(rd());

Scene::Scene(int winWidth, int winHeight)
	: width{ winWidth }, height{ winHeight }
{
	drawMode = 1;
	quadrant = new Quadrant[4]{ Quadrant(0.f, 0.f, 1.f, 1.f), Quadrant(-1.f, 0.f, 0.f, 1.f),
		Quadrant(-1.f, -1.f, 0.f, 0.f), Quadrant(0.f, -1.f, 1.f, 0.f) };
}

void Scene::initialize()
{
	triangleShader = makeShader("C:\\Users\\worl\\Desktop\\Lecture\\2-2\\CG\\CG\\1-8\\triangle_vertex.glsl", "C:\\Users\\worl\\Desktop\\Lecture\\2-2\\CG\\CG\\1-8\\fragment.glsl");
}

void Scene::update()
{
}

void Scene::draw()
{
	{
		std::uniform_real_distribution<float> urd(0.f, 1.f);

		glUseProgram(triangleShader);
		GLint uPosLoc = glGetUniformLocation(triangleShader, "uPos");
		GLint uColorLoc = glGetUniformLocation(triangleShader, "vColor");

		if (uPosLoc < 0)
			std::cerr << "triangle uPos 찾지 못함" << std::endl;

		if (uColorLoc < 0)
			std::cerr << "triangle vColor 찾지 못함" << std::endl;

		for (int i = 0; i < 4; ++i)
			quadrant[i].draw(drawMode, uPosLoc, uColorLoc);
	}
}

void Scene::keyboard(unsigned char key)
{
	switch (key) {
	case 27:	// ESC Key
		delete[] quadrant;
		glutLeaveMainLoop();
		break;

	case 'c':
		std::cout << "ALL CLEAR" << '\n';
		delete[] quadrant;
		quadrant = new Quadrant[4];
		//for (int i = 0; i < triangleList->size(); ++i) {
		//	triangleList[i].clear();
		//}

		break;

	case 'a':
		drawMode = 0;
		break;

	case 'b':
		drawMode = 1;
		break;

	default:
		break;
	}
}

void Scene::keyboardUp(unsigned char key)
{
}

void Scene::specialKeyboard(int key)
{
}

void Scene::specialKeyboardUp(int key)
{
}

void Scene::mouse(int button, int state, int x, int y)
{
	// 화면 업데이트가 된다....

	constexpr int WHEEL_UP = 3;
	constexpr int WHEEL_DOWN = 4;

	if (state == GLUT_DOWN) {
		switch (button) {
		case GLUT_LEFT_BUTTON:
		{
			float xPos = static_cast<float>(x) / width * 2.f - 1.f;
			float yPos = -(static_cast<float>(y) / height * 2.f - 1.f);

			for (int i = 0; i < 4; ++i) {
				if (quadrant[i].isClicked(xPos, yPos)) {
					quadrant[i].clearShapes();
					quadrant[i].insert(xPos, yPos);
					break;
				}
			}

			break;
		}

		case GLUT_RIGHT_BUTTON:
			float xPos = static_cast<float>(x) / width * 2.f - 1.f;
			float yPos = -(static_cast<float>(y) / height * 2.f - 1.f);

			for (int i = 0; i < 4; ++i) {
				if (quadrant[i].isClicked(xPos, yPos)) {
					if (quadrant[i].getListSize() >= 3) {
						std::cout << "Zannen: More Than 3" << '\n';
						break;
					}

					quadrant[i].insert(xPos, yPos);
					break;
				}
			}
			break;
		}

	}
}

void Scene::setWindowSize(int winWidth, int winHeight)
{
	width = winWidth;
	height = winHeight;
}

//int Scene::getMode()
//{
//	return drawMode;
//}

GLuint Scene::makeShader(std::string vertexFilename, std::string fragmentFilename)
{
	// 쉐이더 만들자...

	GLint result;
	GLchar errorLog[512];

	GLuint vertexShader, fragmentShader;

	std::string vertexStr = readFile(vertexFilename);	// 이 변수는 지역변수라서
	const char* vertexSource{ vertexStr.c_str() };		// 이 변수에 저장되려면 위 변수가 소멸되면 안된다..!

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);

	if (!result) {
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader error\n" << errorLog << std::endl;
		//return;
	}

	else {
		std::cout << "vertex shader 컴파일 성공!" << std::endl;
	}

	std::string fragmentStr = readFile(fragmentFilename);
	const char* fragmentSource{ fragmentStr.c_str() };

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);

	if (!result) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: fragment shader error\n" << errorLog << std::endl;
		//return;
	}

	else {
		std::cout << "fragment shader 컴파일 성공!" << std::endl;
	}

	GLuint shaderID = glCreateProgram();
	glAttachShader(shaderID, vertexShader);		// 만든 vertex Shader를 쉐이더 프로그램에 추가해 준다
	glAttachShader(shaderID, fragmentShader);	// 만든 fragment Shader를 쉐이더 프로그램에 추가해 준다
	glLinkProgram(shaderID);					// 추가한 쉐이더들을 하나의 프로그램으로 생성한다!

	glDeleteShader(vertexShader);				// 이제 더이상 vertex Shaer는 필요없다...
	glDeleteShader(fragmentShader);				// 이제 더이상 fragment Shaer는 필요없다...

	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);

	if (!result) {
		glGetProgramInfoLog(shaderID, 512, NULL, errorLog);
		std::cerr << "ERROR: shader program 연결 실패\n" << errorLog << std::endl;
		//return;
	}

	else {
		std::cout << "shader program 생성 성공!" << std::endl;
	}

	glUseProgram(shaderID);						// 최종적으로 만들어진 쉐이더 프로그램을 적용한다!

	return shaderID;
}

std::string Scene::readFile(std::string filename)
{
	std::ifstream in{ filename };

	if (!in) {
		std::cout << filename << "파일 존재하지 않음!" << std::endl;
		exit(1);
	}

	std::string str;
	std::string temp;

	while (std::getline(in, temp)) {
		str += temp;
		str.append(1, '\n');
	}

	return str;
}
