#include <iostream>
#include <fstream>
#include <chrono>
#include "Scene.h"

Scene::Scene(int winWidth, int winHeight)
	: width{ winWidth }, height{ winHeight }
{
	selectMode = 1;
}

void Scene::initialize()
{
	sceneShader = makeShader("C:\\Users\\worl\\Desktop\\Lecture\\2-2\\CG\\CG\\1-9\\vertex.glsl", "C:\\Users\\worl\\Desktop\\Lecture\\2-2\\CG\\CG\\1-9\\fragment.glsl");

	initBuffer();
}

void Scene::update()
{
	for (auto& t : triangles) {
		t.update(selectMode);
	}
}

void Scene::draw()
{
	glUseProgram(sceneShader);

	for (auto& t : triangles) {
		t.draw(sceneShader, VAO);
	}
}

void Scene::keyboard(unsigned char key)
{
	switch (key) {
	case '1':
		selectMode = 1;

		for (int i = 0; i < 4; ++i)
			triangles[i].setMode(selectMode);

		break;

	case '2':
		selectMode = 2;

		for (int i = 0; i < 4; ++i)
			triangles[i].setMode(selectMode);

		break;

	case '3':
		selectMode = 3;

		for (int i = 0; i < 4; ++i)
			triangles[i].setMode(selectMode);

		break;

	case '4':
		selectMode = 4;

		for (int i = 0; i < 4; ++i)
			triangles[i].setMode(selectMode);

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
		case GLUT_LEFT_BUTTON:
			std::cout << "��Ŭ�� : " << x << ", " << y << std::endl;
			std::cout << "OpenGL x ��ǥ�� " << (static_cast<float>(x) / width * 2.f - 1.f) << std::endl;
			std::cout << "OpenGL y ��ǥ�� " << (static_cast<float>(height - y) / height * 2.f - 1.f) << std::endl;
			break;

		case GLUT_MIDDLE_BUTTON:
			std::cout << "��Ŭ�� : " << x << ", " << y << std::endl;
			break;

		case GLUT_RIGHT_BUTTON:
			std::cout << "��Ŭ�� : " << x << ", " << y << std::endl;
			break;

		case WHEEL_UP:
			std::cout << "��  �� : " << x << ", " << y << std::endl;
			break;

		case WHEEL_DOWN:
			std::cout << "�ٴٿ� : " << x << ", " << y << std::endl;
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

void Scene::initBuffer()
{
	glGenVertexArrays(4, VAO);

	float vertexArray[4][15]
	{
		{	// ��
			0.0f, 0.2f,		1.f, 0.f, 0.f,	// 1���� (����(x, y), ����(r, g, b))
			-0.1f, -0.2f,	0.f, 1.f, 0.f,	// 2���� (����(x, y), ����(r, g, b))
			0.1f, -0.2f,	0.f, 0.f, 1.f,	// 3���� (����(x, y), ����(r, g, b))
		},
		{	// �Ʒ�
			0.0f, -0.2f,	1.f, 0.f, 0.f,	// 1���� (����(x, y), ����(r, g, b))
			0.1f, 0.2f,		0.f, 1.f, 0.f,	// 2���� (����(x, y), ����(r, g, b))
			-0.1f, 0.2f,	0.f, 0.f, 1.f,	// 3���� (����(x, y), ����(r, g, b))
		},
		{	// ����
			-0.2f, 0.0f,	1.f, 0.f, 0.f,	// 1���� (����(x, y), ����(r, g, b))
			0.2f, -0.1f,	0.f, 1.f, 0.f,	// 2���� (����(x, y), ����(r, g, b))
			0.2f, 0.1f,		0.f, 0.f, 1.f,	// 3���� (����(x, y), ����(r, g, b))
		},
		{	// ������
			0.2f, 0.0f,		1.f, 0.f, 0.f,	// 1���� (����(x, y), ����(r, g, b))
			-0.2f, 0.1f,	0.f, 1.f, 0.f,	// 2���� (����(x, y), ����(r, g, b))
			-0.2f, -0.1f,	0.f, 0.f, 1.f,	// 3���� (����(x, y), ����(r, g, b))
		}
	};

	for (int i = 0; i < 4; ++i) {
		glBindVertexArray(VAO[i]);

		GLuint VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		// CPU �޸𸮿� �ִ� �����͸�, GPU �޸𸮿� ����
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 15, &vertexArray[i], GL_STATIC_DRAW);

		// �� �����Ͱ� � ����������, �츮�� ���Ǹ� �߱� ������, openGL�� �˷���� �Ѵ�!
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
		// location, ����, Ÿ��, normalized?, ����(����Ʈ), ���ۿ�����
		glEnableVertexAttribArray(0);

		// �� �����Ͱ� � ����������, �츮�� ���Ǹ� �߱� ������, openGL�� �˷���� �Ѵ�!
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, reinterpret_cast<void*>(sizeof(float) * 2));
		glEnableVertexAttribArray(1);
	}
}

GLuint Scene::makeShader(std::string vertexFilename, std::string fragmentFilename)
{
	// ���̴� ������...

	auto m_start = std::chrono::system_clock::now();

	GLint result;
	GLchar errorLog[512];

	GLuint vertexShader, fragmentShader;

	std::string vertexStr = readFile(vertexFilename);	// �� ������ ����������
	const char* vertexSource{ vertexStr.c_str() };		// �� ������ ����Ƿ��� �� ������ �Ҹ�Ǹ� �ȵȴ�..!

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);

	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << vertexFilename << " ERROR: vertex shader error\n" << errorLog << std::endl;
	}

	else {
		std::cout << vertexFilename << " ������ ����!" << std::endl;
	}

	std::string fragmentStr = readFile(fragmentFilename);
	const char* fragmentSource{ fragmentStr.c_str() };

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);

	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << fragmentFilename << " ERROR: fragment shader error\n" << errorLog << std::endl;
	}

	else {
		std::cout << fragmentFilename << " ������ ����!" << std::endl;
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
		std::cerr << vertexFilename << ", " << fragmentFilename << " ERROR: shader program ���� ����\n" << errorLog << std::endl;
		//return;
	}

	else {
		std::cout << vertexFilename << ", " << fragmentFilename << " shader program ���� ����!" << std::endl;
	}
	
	auto m_end = std::chrono::system_clock::now();
	std::chrono::duration<double, std::milli>dur = m_end - m_start;
	std::cout << dur.count() << "ms" << '\n';

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
