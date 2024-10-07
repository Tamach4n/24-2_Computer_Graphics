#include <iostream>
#include <fstream>
#include <random>
#include "Scene.h"

std::random_device rd;
std::default_random_engine dre(rd());

Scene::Scene(int winWidth, int winHeight)
	: width{ winWidth }, height{ winHeight }
{
	selectPolygon = -1;
	end = 0;
}

void Scene::initialize()
{
	pointShader = makeShader("C:\\Users\\worl\\Desktop\\Lecture\\2-2\\CG\\CG\\1-7\\point_vertex.glsl", "C:\\Users\\worl\\Desktop\\Lecture\\2-2\\CG\\CG\\1-7\\fragment.glsl");
	triangleShader = makeShader("C:\\Users\\worl\\Desktop\\Lecture\\2-2\\CG\\CG\\1-7\\triangle_vertex.glsl", "C:\\Users\\worl\\Desktop\\Lecture\\2-2\\CG\\CG\\1-7\\fragment.glsl");
	rectangleShader = makeShader("C:\\Users\\worl\\Desktop\\Lecture\\2-2\\CG\\CG\\1-7\\rectangle_vertex.glsl", "C:\\Users\\worl\\Desktop\\Lecture\\2-2\\CG\\CG\\1-7\\fragment.glsl");
}

void Scene::update()
{
}

void Scene::draw()
{
	{
		glUseProgram(pointShader);
		glPointSize(5.f);

		GLint uPosLoc = glGetUniformLocation(pointShader, "uPos");

		if (uPosLoc < 0)
			std::cerr << "point uPos ã�� ����" << std::endl;

		for (const auto& p : pointList) {
			glUniform2f(uPosLoc, p.first, p.second);
			glDrawArrays(GL_POINTS, 0, 1);
		}
	}
	{
		glUseProgram(triangleShader);
		GLint uPosLoc = glGetUniformLocation(\
			triangleShader, "uPos");

		if (uPosLoc < 0)
			std::cerr << "triangle uPos ã�� ����" << std::endl;

		for (const auto& p : triangleList) {
			glUniform2f(uPosLoc, p.first, p.second);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
	}
	{
		glUseProgram(rectangleShader);
		GLint uPosLoc = glGetUniformLocation(rectangleShader, "uPos");

		if (uPosLoc < 0)
			std::cerr << "rectangle uPos ã�� ����" << std::endl;

		for (const auto& p : rectangleList) {
			glUniform2f(uPosLoc, p.first, p.second);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		for (const auto& p : lineList) {
			glUniform2f(uPosLoc, p.first, p.second);
			glLineWidth(2.f);
			glDrawArrays(GL_LINES, 3, 2);
		}
	}
}

void Scene::keyboard(unsigned char key)
{
	switch (key) {
	case 27:	// ESC Key
		glutLeaveMainLoop();
		break;

	case 'p':
		std::cout << "Mode: Point\n";
		selectPolygon = 0;
		break;

	case 'l':
		std::cout << "Mode: Line\n";
		selectPolygon = 1;
		break;

	case 't':
		std::cout << "Mode: Triangle\n";
		selectPolygon = 2;
		break;

	case 'r':
		std::cout << "Mode: Rectangle\n";
		selectPolygon = 3;
		break;

	case 'c':
		std::cout << "ALL CLEAR" << '\n';

		for (int i = 0; i < end; ++i) {
			select[i]->first = 0.f;
			select[i]->second = 0.f;
		}

		end = 0; 
		pointList.clear();
		triangleList.clear();
		rectangleList.clear();
		break;

	case 'w':
	{
		moveShape(0, 1);
		break;
	}

	case 'a':
	{
		moveShape(-1, 0);
		break;
	}

	case 's':
	{
		moveShape(0, -1);
		break;
	}

	case 'd': {
		moveShape(1, 0);
		break;
	}

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
	// ȭ�� ������Ʈ�� �ȴ�....

	constexpr int WHEEL_UP = 3;
	constexpr int WHEEL_DOWN = 4;

	switch (state) {
	case GLUT_DOWN:
		switch (button) {
		case GLUT_LEFT_BUTTON: {
			if (end >= 10) {
				std::cout << "Zannen: More Than 10" << '\n';
				break;
			}

			if (selectPolygon >= 0) {
				float xPos = static_cast<float>(x) / width * 2.f - 1.f;
				float yPos = -(static_cast<float>(y) / height * 2.f - 1.f);

				switch (selectPolygon) {
				case 0:
					pointList.push_back({ xPos, yPos });
					select[end] = &pointList.back();
					break;

				case 1:
					lineList.push_back({ xPos,yPos });
					select[end] = &lineList.back();
					break;

				case 2:
					triangleList.push_back({ xPos, yPos });
					select[end] = &triangleList.back();
					break;

				case 3:
					rectangleList.push_back({ xPos, yPos });
					select[end] = &rectangleList.back();					
					break;
				}

				std::cout << end + 1 << " : " << select[end]->first << ", " << select[end]->second << '\n';
				++end;
			}

			break;
		}

		case GLUT_MIDDLE_BUTTON:
			break;

		case GLUT_RIGHT_BUTTON:
			break;

		case WHEEL_UP:
			break;

		case WHEEL_DOWN:
			break;
		}

		break;

	case GLUT_UP:
		switch (button) {
		case GLUT_LEFT_BUTTON:
			break;

		case GLUT_RIGHT_BUTTON:
			break;

		case GLUT_MIDDLE_BUTTON:
			break;
		}

		break;
	}
}

void Scene::setWindowSize(int winWidth, int winHeight)
{
	width = winWidth;
	height = winHeight;
}

GLuint Scene::makeShader(std::string vertexFilename, std::string fragmentFilename)
{
	// ���̴� ������...

	GLint result;
	GLchar errorLog[512];

	GLuint vertexShader, fragmentShader;

	std::string vertexStr = readFile(vertexFilename);	// �� ������ ����������
	const char* vertexSource{ vertexStr.c_str() };		// �� ������ ����Ƿ��� �� ������ �Ҹ�Ǹ� �ȵȴ�..!

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
		std::cout << "vertex shader ������ ����!" << std::endl;
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
		std::cout << "fragment shader ������ ����!" << std::endl;
	}

	GLuint shaderID = glCreateProgram();
	glAttachShader(shaderID, vertexShader);		// ���� vertex Shader�� ���̴� ���α׷��� �߰��� �ش�
	glAttachShader(shaderID, fragmentShader);	// ���� fragment Shader�� ���̴� ���α׷��� �߰��� �ش�
	glLinkProgram(shaderID);					// �߰��� ���̴����� �ϳ��� ���α׷����� �����Ѵ�!

	glDeleteShader(vertexShader);				// ���� ���̻� vertex Shaer�� �ʿ����...
	glDeleteShader(fragmentShader);				// ���� ���̻� fragment Shaer�� �ʿ����...

	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);

	if (!result) {
		glGetProgramInfoLog(shaderID, 512, NULL, errorLog);
		std::cerr << "ERROR: shader program ���� ����\n" << errorLog << std::endl;
		//return;
	}

	else {
		std::cout << "shader program ���� ����!" << std::endl;
	}

	glUseProgram(shaderID);						// ���������� ������� ���̴� ���α׷��� �����Ѵ�!

	return shaderID;
}

std::string Scene::readFile(std::string filename)
{
	std::ifstream in{ filename };

	if (!in) {
		std::cout << filename << "���� �������� ����!" << std::endl;
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

int Scene::getEnd()
{
	return end;
}

int Scene::selectShape()
{
	std::uniform_int_distribution<> uid(0, end - 1);

	return uid(dre);
}

void Scene::moveShape(int dirX, int dirY)
{
	if (getEnd() == 0) {
		std::cout << "No Shapes" << '\n';
		return;
	}

	int sel = selectShape();

	std::cout << sel << '\n';

	GLfloat disX = 0.05f;
	GLfloat disY = 0.05f;

	select[sel]->first += disX * dirX;
	select[sel]->second += disY * dirY;
}
